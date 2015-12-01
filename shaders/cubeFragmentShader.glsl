#version 330 core

#define DIR 1
#define SPOT 2
#define POINT 3

float ambientStrength = 0.5;
float diffStrength = .8;
float specStrength = .8;

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

vec3 cubeColor = vec3(1.0, 0.32, 1.0);

struct Light {
	int type;
	float intense;
	int disabled;
	vec3 dir;
	vec3 color;
	vec3 pos;
    float spotAngle;
};

uniform Light lights[32];

vec3 Diffuse(vec3 ldir, vec3 norm, vec3 color, float strength) {
	float kdot = max(dot(ldir, norm), 0.0);
	return strength * kdot * M.diffuse * diffStrength * color; 
}

float A = 0.0028, B = 0.09, C = 0.027;
float Atten(vec3 lpos) {
	float D = length(lpos - fragPos);
	return 1.0 / (A*D*D + B*D + C);
}

vec3 Specular(vec3 lpos, float lspec) {
	vec3 ldir = normalize(lpos - fragPos);
	vec3 eyee = normalize(eye - fragPos);
	float lambert = dot(ldir, normal);
	if (lambert > 0.0) {
		vec3 refl = reflect(-ldir, normal);
		float specular = pow(max(dot(refl, eyee), 0.0), M.shine);
		return lspec * M.specular * specular;
	}
	return vec3(0.0, 0.0, 0.0);
}

vec3 I = vec3(1.0, 1.0, 1.0);
vec3 ambient = I * 0;
vec3 diffuse = vec3(0., 0., 0.);
vec3 specular = vec3(0., 0., 0.);
vec3 result = vec3(0.0, 0.0, 0.0);

void main(void)
{	
	M.diffuse = I * 0.4;
	M.ambient = I * 0.4;
	M.specular = I * 0.2;
	M.shine = 4.0;
	
	for(int i = 0; i < lightamt; i++) {
		if (lights[i].disabled != 0)
			continue;
		if (lights[i].type == SPOT) {
			vec3 ltof = normalize(fragPos - lights[i].pos);
			float angle = dot(ltof, normalize(lights[i].dir));
			ambient = M.ambient * ambientStrength; 
			diffuse = Diffuse(normalize(lights[i].pos - fragPos), normal, lights[i].color, 1.);
			specular = Specular(lights[i].pos, 1);
			result += lights[i].intense*(ambient + diffuse + specular)*(angle > lights[i].spotAngle? 0.0 : Atten(lights[i].pos));
			
		} else if (lights[i].type == POINT) {
			ambient = M.ambient * ambientStrength;
			diffuse = Diffuse(normalize(lights[i].pos - fragPos), normal, lights[i].color, 1.);
			specular = Specular(lights[i].pos, 1);
			result += lights[i].intense*(ambient + diffuse + specular)*Atten(lights[i].pos);
			
		} else if (lights[i].type == DIR) {
			ambient = M.ambient * ambientStrength;
			diffuse = Diffuse(normalize(lights[i].dir), normal, lights[i].color, 1.);			
			result += lights[i].intense*(ambient + diffuse);
		}
	}
	color = vec4(result*cubeColor, 1.0);
}