#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

struct drawable {

	GLuint vao;
	GLuint program;

	void (*draw)(struct drawable *);

};

#endif
