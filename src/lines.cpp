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

void Triangles::SetTriangle(int v1, floatv x1, floatv y1, floatv z1, floatv x2, floatv y2, floatv z2, floatv x3, floatv y3, floatv z3) {
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

void Lines::Draw(Program prog) {
	glBindVertexArray(vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, (vamt*3) * sizeof(float), vertex, GL_STATIC_DRAW);
	prog.EnableVertexAttribArray();
	prog.VertexAttribPointer(3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(mode, 0, vamt*3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
