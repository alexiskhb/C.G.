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

float A = 0.0028, B = 0.09, C = 0.027;

vec3 cubeColor;

struct Light {
	int type;
	vec3 dir;
	vec3 color;
	vec3 pos;
    float spotAngle;
};

uniform Light lights[100];

vec3 result;

vec3 Diffuse(vec3 dir, vec3 norm, vec3 color, float strength) {
	vec3 res;
	float kdot = max(dot(dir, norm), 0.0);
	res = strength * kdot * M.diffuse * diffStrength * color; 
	return res;
}

float Atten(vec3 lpos, vec3 ppos) {
	float D = length(lpos - ppos);
	return 1.0 / (A*D*D + B*D + C);
}

vec3 Specular(vec3 lpos, float lspec, vec3 fpos) {
	vec3 ldir = normalize(lpos - fpos);
	vec3 eyee = normalize(-fpos);
	vec3 res = vec3(0.0, 0.0, 0.0);
	float lambert = dot(ldir, normal);
	if (lambert > 0.0) {
		vec3 refl = reflect(-ldir, normal);
		float specular = pow(max(dot(refl, eyee), 0.0), M.shine);
		res = lspec * M.specular * specular;
	}
	return res;
}

void main(void)
{
	vec3 norm = normalize(normal);
	//cubeColor = vec3(1.0, 1.0, 1.0);
	cubeColor = vec3(1.0, 0.32, 1.0);
    //cubeColor = vec3(0.978, 0.488, 0.0);
	
	vec3 I = vec3(1.0, 1.0, 1.0);
	M.diffuse = I * 0.4;
	M.ambient = I * 0.4;
	M.specular = I * 0.9;
	M.shine = 64.0;
	
	vec3 ambient = I * 0;
	vec3 diffuse = vec3(0., 0., 0.);
	vec3 specular = vec3(0., 0., 0.);
	result = vec3(0.0, 0.0, 0.0);	
	for(int i = 0; i < lightamt; i++) {
		if (lights[i].type == POINT) {
			ambient = M.ambient * ambientStrength;
			diffuse = Diffuse(normalize(lights[i].pos - fragPos), norm, lights[i].color, 1.);
			specular = Specular(lights[i].pos, 1, fragPos);
			result += (ambient + diffuse + specular)*Atten(lights[i].pos, fragPos);
			
		} else if (lights[i].type == DIR) {
			ambient = M.ambient * ambientStrength;
			diffuse = Diffuse(normalize(lights[i].dir), norm, lights[i].color, 1);			
			result += ambient + diffuse;
						
		} else  if (lights[i].type == SPOT) {
			ambient = M.ambient * ambientStrength; 
			diffuse = Diffuse(normalize(lights[i].pos - fragPos), norm, lights[i].color, 1.);
			specular = Specular(lights[i].pos, 1, fragPos);
			result += (ambient + diffuse + specular)*Atten(lights[i].pos, fragPos);;
		}
	}
	color = vec4(result*cubeColor, 1.0);
}