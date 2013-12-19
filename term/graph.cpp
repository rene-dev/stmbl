#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/select.h>
#include <string.h>
#include <sys/types.h>
#include <termios.h>
#include <fcntl.h>
#include <signal.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_utils.h"
#include "stlink-common.h"

#define LEN 500

GLuint program;
GLint attribute_coord2d;
GLint uniform_color;
GLint uniform_transform;

float offset_x = 0;
float scale_x = 1;
int x_pos = 0;

struct point {
	GLfloat x;
	GLfloat y;
};

point graph[LEN];
GLuint vbo[3];
GLFWwindow* window;

const int border = 10;
const int ticksize = 10;

#define STLINKY_MAGIC 0xDEADF00D

#define READ_UINT32_LE(buf)  ((uint32_t) (   buf[0]         \
                                           | buf[1] <<  8   \
                                           | buf[2] << 16   \
                                           | buf[3] << 24))

typedef struct {
        stlink_t *sl;
        uint32_t off;
        size_t bufsize;
}stlinky;

/* Detects stlinky in RAM, returns handler */
stlinky*  stlinky_detect(stlink_t* sl)
{
        static const uint32_t sram_base = 0x20000000;
        stlinky* st = (stlinky*)malloc(sizeof(stlinky));
        int multiple=0;
        st->sl = sl;
        printf("sram: 0x%x bytes @ 0x%zx\n", sl->sram_base, sl->sram_size);
        uint32_t off;
        for (off = 0; off < sl->sram_size; off += 4) {
                stlink_read_mem32(sl, sram_base + off, 4);
                if (STLINKY_MAGIC == READ_UINT32_LE(sl->q_buf))
                {
                        if (multiple > 0)
                        printf("WARNING: another ");
                        printf("stlinky detected at 0x%x\n", sram_base + off);
                        st->off = sram_base + off;
                        stlink_read_mem32(sl, st->off + 4, 4);
                        st->bufsize = (size_t) *(unsigned char*) sl->q_buf;
                        printf("stlinky buffer size 0x%zu \n", st->bufsize);
                        multiple++;
                }
        }
        if (multiple > 0) {
                if (multiple > 1) {
                        printf("Using last stlinky structure detected\n");
                }
                return st;
        }
        return NULL;
}

int stlinky_canrx(stlinky *st)
{
        stlink_read_mem32(st->sl, st->off+4, 4);
        unsigned char tx = (unsigned char) st->sl->q_buf[1];
        return (int) tx;
}

size_t stlinky_rx(stlinky *st, char* buffer)
{
        unsigned char tx = 0;
        while(tx == 0) {
                stlink_read_mem32(st->sl, st->off+4, 4);
                tx = (unsigned char) st->sl->q_buf[1];
        }
        size_t rs = tx + (4 - (tx % 4)); /* voodoo */
        stlink_read_mem32(st->sl, st->off+8, rs);
        memcpy(buffer, st->sl->q_buf, (size_t) tx);
        *st->sl->q_buf=0x0;
        stlink_write_mem8(st->sl, st->off+5, 1);
        return (size_t) tx;
}

size_t stlinky_tx(stlinky *st, char* buffer, size_t sz)
{
        unsigned char rx = 1;
        while(rx != 0) {
                stlink_read_mem32(st->sl, st->off+4, 4);
                rx = (unsigned char) st->sl->q_buf[2];
        }
        memcpy(st->sl->q_buf, buffer, sz);
        size_t rs = sz + (4 - (sz % 4)); /* voodoo */
        stlink_write_mem32(st->sl, st->off+8+st->bufsize, rs);
        *st->sl->q_buf=(unsigned char) sz;
        stlink_write_mem8(st->sl, st->off+6, 1);
        return (size_t) rx;
}

int kbhit()
{
        struct timeval tv;
        fd_set fds;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
        select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
        return FD_ISSET(STDIN_FILENO, &fds);
}

void nonblock(int state)
{
        struct termios ttystate;

        //get the terminal state
        tcgetattr(STDIN_FILENO, &ttystate);

        if (state==1)
        {
                //turn off canonical mode
                ttystate.c_lflag &= ~ICANON;
                ttystate.c_lflag &= ~ECHO;
                //minimum of number input read.
                ttystate.c_cc[VMIN] = 1;
        }
        else if (state==0)
        {
                //turn on canonical mode
                ttystate.c_lflag |= ICANON | ECHO;
        }
        //set the terminal attributes.
        tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

}

static int keep_running = 1;
static int sigcount=0;
void cleanup(int dummy)
{
        (void) dummy;
        sigcount++;
        keep_running = 0;
        printf("\n\nGot a signal\n");
        if (sigcount==2) {
                printf("\n\nGot a second signal - bailing out\n");
                exit(1);
        }
}

int init_resources() {
	program = create_program("graph.v.glsl", "graph.f.glsl");
	if (program == 0)
		return 0;

	attribute_coord2d = get_attrib(program, "coord2d");
	uniform_transform = get_uniform(program, "transform");
	uniform_color = get_uniform(program, "color");

	if (attribute_coord2d == -1 || uniform_transform == -1 || uniform_color == -1)
		return 0;

	// Create the vertex buffer object
	glGenBuffers(3, vbo);

	// Create a VBO for the border
	static const point border[4] = { {-1, -1}, {1, -1}, {1, 1}, {-1, 1} };
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof border, border, GL_STATIC_DRAW);

	return 1;
}

// Create a projection matrix that has the same effect as glViewport().
// Optionally return scaling factors to easily convert normalized device coordinates to pixels.
//
glm::mat4 viewport_transform(float x, float y, float width, float height, float *pixel_x = 0, float *pixel_y = 0) {
	// Map OpenGL coordinates (-1,-1) to window coordinates (x,y),
	// (1,1) to (x + width, y + height).

	// First, we need to know the real window size:
	float window_width = 640;//glutGet(GLUT_WINDOW_WIDTH);
	float window_height = 480;//glutGet(GLUT_WINDOW_HEIGHT);

	// Calculate how to translate the x and y coordinates:
	float offset_x = (2.0 * x + (width - window_width)) / window_width;
	float offset_y = (2.0 * y + (height - window_height)) / window_height;

	// Calculate how to rescale the x and y coordinates:
	float scale_x = width / window_width;
	float scale_y = height / window_height;

	// Calculate size of pixels in OpenGL coordinates
	if (pixel_x)
		*pixel_x = 2.0 / width;
	if (pixel_y)
		*pixel_y = 2.0 / height;

	return glm::scale(glm::translate(glm::mat4(1), glm::vec3(offset_x, offset_y, 0)), glm::vec3(scale_x, scale_y, 1));
}

void display(int window_width,int window_height) {
	glUseProgram(program);

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	/* ---------------------------------------------------------------- */
	/* Draw the graph */

	// Set our viewport, this will clip geometry
	glViewport(border + ticksize, border + ticksize, window_width - border * 2 - ticksize, window_height - border * 2 - ticksize);

	// Set the scissor rectangle,this will clip fragments
	glScissor(border + ticksize, border + ticksize, window_width - border * 2 - ticksize, window_height - border * 2 - ticksize);

	glEnable(GL_SCISSOR_TEST);

	// Set our coordinate transformation matrix
	glm::mat4 transform = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(scale_x, 1, 1)), glm::vec3(offset_x, 0, 0));
	glUniformMatrix4fv(uniform_transform, 1, GL_FALSE, glm::value_ptr(transform));

	// Set the color to red
	GLfloat red[4] = { 1, 0, 0, 1 };
	glUniform4fv(uniform_color, 1, red);

	// Draw using the vertices in our vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glEnableVertexAttribArray(attribute_coord2d);
	glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_LINE_STRIP, 0, LEN);
    //glDrawArrays(GL_POINTS, 0, LEN);

	// Stop clipping
	glViewport(0, 0, window_width, window_height);
	glDisable(GL_SCISSOR_TEST);

	/* ---------------------------------------------------------------- */
	/* Draw the borders */

	float pixel_x, pixel_y;

	// Calculate a transformation matrix that gives us the same normalized device coordinates as above
	transform = viewport_transform(border + ticksize, border + ticksize, window_width - border * 2 - ticksize, window_height - border * 2 - ticksize, &pixel_x, &pixel_y);

	// Tell our vertex shader about it
	glUniformMatrix4fv(uniform_transform, 1, GL_FALSE, glm::value_ptr(transform));

	// Set the color to black
	GLfloat black[4] = { 0, 0, 0, 1 };
	glUniform4fv(uniform_color, 1, black);

	// Draw a border around our graph
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	/* ---------------------------------------------------------------- */
	/* Draw the y tick marks */

	point ticks[42];

	for (int i = 0; i <= 20; i++) {
		float y = -1 + i * 0.1;
		float tickscale = (i % 10) ? 0.5 : 1;

		ticks[i * 2].x = -1;
		ticks[i * 2].y = y;
		ticks[i * 2 + 1].x = -1 - ticksize * tickscale * pixel_x;
		ticks[i * 2 + 1].y = y;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof ticks, ticks, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_LINES, 0, 42);

	/* ---------------------------------------------------------------- */
	/* Draw the x tick marks */

	float tickspacing = 0.1 * powf(10, -floor(log10(scale_x)));	// desired space between ticks, in graph coordinates
	float left = -1.0 / scale_x - offset_x;	// left edge, in graph coordinates
	float right = 1.0 / scale_x - offset_x;	// right edge, in graph coordinates
	int left_i = ceil(left / tickspacing);	// index of left tick, counted from the origin
	int right_i = floor(right / tickspacing);	// index of right tick, counted from the origin
	float rem = left_i * tickspacing - left;	// space between left edge of graph and the first tick

	float firsttick = -1.0 + rem * scale_x;	// first tick in device coordinates

	int nticks = right_i - left_i + 1;	// number of ticks to show

	if (nticks > 21)
		nticks = 21;	// should not happen

	for (int i = 0; i < nticks; i++) {
		float x = firsttick + i * tickspacing * scale_x;
		float tickscale = ((i + left_i) % 10) ? 0.5 : 1;

		ticks[i * 2].x = x;
		ticks[i * 2].y = -1;
		ticks[i * 2 + 1].x = x;
		ticks[i * 2 + 1].y = -1 - ticksize * tickscale * pixel_y;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof ticks, ticks, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_LINES, 0, nticks * 2);

	// And we are done.

	glDisableVertexAttribArray(attribute_coord2d);
}

void addpoint(float y){
	float x = (x_pos - LEN/2) / (LEN/2.0);
    graph[x_pos].x = x;
    graph[x_pos].y = y;
	graph[(x_pos+1)%LEN].y = 0;
    x_pos++;
    x_pos%=LEN;
}

void render(void) {
	int window_width;
	int window_height;
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof graph, graph, GL_STREAM_DRAW);
    glfwGetFramebufferSize(window, &window_width, &window_height);
    display(window_width,window_height);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main(int ac, char** av) {
    struct itimerval it_val;	/* for setting itimer */
    stlink_t* sl;
    stlinky *st;
    int add = 0;
    
    if (signal(SIGALRM, (void (*)(int)) render) == SIG_ERR) {
        perror("Unable to catch SIGALRM");
        exit(1);
     }
    
    it_val.it_value.tv_sec =     0;
    it_val.it_value.tv_usec =    17000;	
    it_val.it_interval = it_val.it_value;
        
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "Graph", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
	
    if(!init_resources())
        exit(EXIT_FAILURE);        
    
	for (int i = 0; i < LEN; i++) {
		float x = (i - LEN/2) / (LEN/2.0);
		graph[i].x = x;
		graph[i].y = 0;
	}
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof graph, graph, GL_DYNAMIC_DRAW);
    

    
    sl = stlink_open_usb(10, 1);
            if (sl != NULL) {
                    printf("ST-Linky proof-of-concept terminal :: Created by Necromant for lulz\n");
                    stlink_version(sl);
                    stlink_enter_swd_mode(sl);
                    printf("chip id: %#x\n", sl->chip_id);
                    printf("core_id: %#x\n", sl->core_id);

                    cortex_m3_cpuid_t cpuid;
                    stlink_cpu_id(sl, &cpuid);
                    printf("cpuid:impl_id = %0#x, variant = %#x\n", cpuid.implementer_id, cpuid.variant);
                    printf("cpuid:part = %#x, rev = %#x\n", cpuid.part, cpuid.revision);

                    stlink_reset(sl);
                    stlink_force_debug(sl);
                    stlink_run(sl);
                    stlink_status(sl);

                    /* wait for device to boot */
                    /* TODO: Make timeout adjustable via command line */
                    sleep(1);

                    if(ac == 1){ 
                            st = stlinky_detect(sl);
                    }else if(ac == 2){
                            st = (stlinky*)malloc(sizeof(stlinky));
                            st->sl = sl;
                            st->off = (int)strtol(av[1], NULL, 16);
                            printf("using stlinky at 0x%x\n", st->off);
                            stlink_read_mem32(sl, st->off + 4, 4);
                            st->bufsize = (size_t) *(unsigned char*) sl->q_buf;
                            printf("stlinky buffer size 0x%zu \n", st->bufsize);
                    }else{
                            //goto bailout;
                    }
                    if (st == NULL)
                    {
                            printf("stlinky magic not found in sram :(\n");
                            //goto bailout;
                    }
                    char* rxbuf = (char*)malloc(st->bufsize);
                    char* txbuf = (char*)malloc(st->bufsize);
                    size_t tmp;
                    nonblock(1);
                    int fd = fileno(stdin);
                    int saved_flags = fcntl(fd, F_GETFL);
                    fcntl(fd, F_SETFL, saved_flags & ~O_NONBLOCK);
                    signal(SIGINT, cleanup);
                    printf("Entering interactive terminal. CTRL+C to exit\n\n\n");
                    /*if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
                        perror("error calling setitimer()");
                        exit(1);
                    }*/
                    add = 0;
                    while(!glfwWindowShouldClose(window)) {
                        if(add == 100){
                            add = 0;
                            render();
                        }
                        add++;
                            if (stlinky_canrx(st)) {
                                    tmp = stlinky_rx(st, rxbuf);

                                    if(rxbuf[0] == 0x11)
                                        addpoint(rxbuf[1]/128.0f);
                                    else{
                                        fwrite(rxbuf,tmp,1,stdout);
                                        fflush(stdout);
                                    }
                            }
                            if (kbhit()) {
                                    tmp = read(fd, txbuf, st->bufsize);
                                    stlinky_tx(st,txbuf,tmp);
                            }
                            if (!keep_running)
                                    break;
                    }
            }
    //bailout:
    //            nonblock(0);
    //    stlink_exit_debug_mode(sl);
    //    stlink_close(sl);
    //TODO: stop timer
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
	return 0;
}
