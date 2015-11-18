#include "lines.hpp"

Lines::~Lines() {
	delete [] this->lines;
}

void Lines::SetLine(int st, floatv x1, floatv z1, floatv x2, floatv z2) {
	st *= 6;
	int e = st + 3;
	lines[st    ] = x1;
	lines[st + 1] = 0.;
	lines[st + 2] = z1;
	lines[e     ] = x2;
	lines[e  + 1] = 0.;
	lines[e  + 2] = z2;
}
void Lines::SetLine(int st, floatv x1, floatv y1, floatv z1, floatv x2, floatv y2, floatv z2) {
	SetLine(st, x1, z1, x2, z2);
	st *= 6;
	int e = st + 3;
	lines[st + 1] = y1;
	lines[e  + 1] = y2;
}

void Lines::Draw(Program prog) {
	glBindVertexArray(vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, (lamt*2*3) * sizeof(float), lines, GL_STATIC_DRAW);
	prog.EnableVertexAttribArray();
	prog.VertexAttribPointer(3, GL_FLOAT, GL_FALSE, 0, 0);
	for(int i = 0; i < lamt; i++)
		glDrawArrays(GL_LINES, i*2, 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
