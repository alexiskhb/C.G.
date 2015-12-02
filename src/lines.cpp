#include "lines.hpp"

Lines::~Lines() {
	delete [] this->vertex;
}

void Lines::SetLine(int st, floatv x1, floatv z1, floatv x2, floatv z2) {
	st *= 6;
	int e = st + 3;
	vertex[st    ] = x1;
	vertex[st + 1] = 0.;
	vertex[st + 2] = z1;
	vertex[e     ] = x2;
	vertex[e  + 1] = 0.;
	vertex[e  + 2] = z2;
}
void Lines::SetLine(int st, floatv x1, floatv y1, floatv z1, floatv x2, floatv y2, floatv z2) {
	SetLine(st, x1, z1, x2, z2);
	st *= 6;
	int e = st + 3;
	vertex[st + 1] = y1;
	vertex[e  + 1] = y2;
}

void Cube::SetTriangle(int v1, floatv x1, floatv y1, floatv z1, floatv x2, floatv y2, floatv z2, floatv x3, floatv y3, floatv z3) {
	v1 *= 9;
	int v2 = v1 + 3, v3 = v2 + 3;
	vertex[v1    ] = x1;
	vertex[v1 + 1] = y1;
	vertex[v1 + 2] = z1;

	vertex[v2    ] = x2;
	vertex[v2 + 1] = y2;
	vertex[v2 + 2] = z2;

	vertex[v3    ] = x3;
	vertex[v3 + 1] = y3;
	vertex[v3 + 2] = z3;
}

void Lines::FillBuffer(Buffer *buff, Program prog) {
	glBindBuffer(GL_ARRAY_BUFFER, buff->vbo);
	glBufferData(GL_ARRAY_BUFFER, (vamt*3) * sizeof(float), vertex, GL_STATIC_DRAW);
	glBindVertexArray(buff->vao);
	buff->first = 0;
	buff->count = vamt*3;
	buff->mode = mode;
	glVertexAttribPointer(glGetAttribLocation(prog.handler, "vertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(prog.handler, "vertexPosition"));
}

void Cube::FillIndexBuffer(Buffer *buff, Program prog) {
	for(int i = 0; i < 36; i++)
		indexes[i] = i;
	prog.Use();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff->via);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned short int), indexes, GL_STATIC_DRAW);
}

void Cube::FillBuffer(Buffer *buff, Program prog) {
	glBindBuffer(GL_ARRAY_BUFFER, buff->vbo);
	glBufferData(GL_ARRAY_BUFFER, 2*(36*3) * sizeof(float), vertex, GL_STATIC_DRAW);
	glBindVertexArray(buff->vao);
	buff->first = 0;
	buff->count = vamt*3;
	buff->mode = mode;
	glVertexAttribPointer(glGetAttribLocation(prog.handler, "vertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(prog.handler, "vertexPosition"));
	glVertexAttribPointer(glGetAttribLocation(prog.handler, "norm"), 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(36*3*sizeof(float)));
	glEnableVertexAttribArray(glGetAttribLocation(prog.handler, "norm"));
	glBindVertexArray(0);
}

void Plane::FillBuffer(Buffer *buff, Program prog) {
	glBindBuffer(GL_ARRAY_BUFFER, buff->vbo);
	glBufferData(GL_ARRAY_BUFFER, 2*(4*3) * sizeof(float), vertex_normals, GL_STATIC_DRAW);
	glBindVertexArray(buff->vao);
	buff->first = 0;
	buff->count = 4*3;
	buff->mode = GL_TRIANGLES;
	glVertexAttribPointer(glGetAttribLocation(prog.handler, "vertexPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(prog.handler, "vertexPosition"));
	glVertexAttribPointer(glGetAttribLocation(prog.handler, "norm"), 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(4*3*sizeof(float)));
	glEnableVertexAttribArray(glGetAttribLocation(prog.handler, "norm"));
	glBindVertexArray(0);
}
void Plane::FillIndexBuffer(Buffer *buff, Program prog) {
	prog.Use();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff->via);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned short int), indexes, GL_STATIC_DRAW);
}
