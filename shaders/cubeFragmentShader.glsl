#version 330 core

#define DIR 1
#define SPOT 2
#define POINT 3

float ambientStrength = 0.5;
float diffStrength = .8;
float specStrength = .4;

uniform sampler2D tex;
in vec2 texCoord;

uniform int lightamt;
uniform vec3 eye;

out vec4 color;

in vec3 fragPos;
in vec3 normal;

struct material {
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    float shine;
} M;
//0.32
vec3 cubeColor = vec3(1.0, 1.0, 1.0);

struct Light {
	int type;
	float intense;
	int disabled;
	vec3 dir;
	vec3 color;
	vec3 pos;
    float spotAngle;
};

uniform Light lights[200];

vec3 Diffuse(vec3 ldir, vec3 norm, vec3 color, float strength) {
	float kdot = max(dot(ldir, norm), 0.0);
	return strength * kdot * M.diffuse * diffStrength * color; 
}

float A = 0.0028, B = 0.09, C = 0.027;
float Atten(vec3 lpos) {
	float D = length(lpos - fragPos);
	return 1.0 / (A*D*D + B*D + C);
}

vec3 ldir;
vec3 eyee;
vec3 refl;
vec3 Specular(vec3 lpos) {
	ldir = normalize(lpos - fragPos);
	eyee = normalize(eye - fragPos);
	float lambert = dot(ldir, normal);
	if (lambert > 0.0) {
		refl = reflect(-ldir, normal);
		float specular = pow(max(dot(refl, eyee), 0.0), M.shine);
		return M.specular * specular;
	}
	return vec3(0.0, 0.0, 0.0);
}

vec3 I = vec3(1.0, 1.0, 1.0);
vec3 ambient = I * 0;
vec3 diffuse = vec3(0., 0., 0.);
vec3 specular = vec3(0., 0., 0.);
vec3 result = vec3(0.0, 0.0, 0.0);

int gtype;
vec3 ltof;
float atten;
float D;
float lambert;
float spec;
void main(void)
{
	M.diffuse = I * 0.4;
	M.ambient = I * 0.4;
	M.specular = I * 0.4;
	M.shine = 64.0;
	
	ambient = M.ambient * ambientStrength;
	for(int i = 0; i < lightamt; i++) {
		gtype = lights[i].type;
		D = length(lights[i].pos - fragPos);
		if (gtype == SPOT) {
			ltof = normalize(fragPos - lights[i].pos);
			float angle = dot(ltof, normalize(lights[i].dir));			
			atten = (angle > lights[i].spotAngle? 0.0 : (1.0 / (A*D*D + B*D + C)));
			specular = I*0;
			ldir = normalize(lights[i].pos - fragPos);
		}
		if (gtype == POINT) {
			atten = 1.0 / (A*D*D + B*D + C);
			ldir = normalize(lights[i].pos - fragPos);	
			eyee = normalize(eye - fragPos);
			lambert = dot(ldir, normal);
			if (lambert > 0.0) {
				refl = reflect(-ldir, normal);
				spec = pow(max(dot(refl, eyee), 0.0), M.shine);
			} else
				spec = 0;
			specular = M.specular * spec;
		}
		if (gtype == DIR) {
			specular = I*0;			
			ldir = normalize(lights[i].dir);
			atten = 1;
		}
		diffuse = max(dot(ldir, normal), 0.0) * M.diffuse * lights[i].color;
		result += lights[i].intense*(ambient + diffuse + specular)*atten;
	}
	//color = vec4(result*cubeColor, 1.0);
	color = texture(tex, texCoord)*vec4(result*cubeColor, 1.0);
}