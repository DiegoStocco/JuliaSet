#version 330 core

uniform vec2 u_screenSize;
uniform vec2 u_constant;

uniform int u_maxIterCount;

vec3 lerp(vec3 v1, vec3 v2, float t) {
	return v1 + t*(v2 - v1);
}

vec2 SingleIter(vec2 z, vec2 c) {
	float re = z.x * z.x - z.y * z.y;
	float im = 2 * z.x * z.y;
	return vec2(re, im) + c;
}

int iterCount(vec2 start, vec2 c, int maxCount) {
	int iterCount = 0;
	vec2 curr = start;
	while(iterCount <= maxCount && length(curr) < 2) {
		curr = SingleIter(curr, c);
		iterCount++;
	}
	return iterCount;
}


void main() {
	vec3 color1 = vec3(0.0, 0.0, 0.0);
	vec3 color2 = vec3(1.0, 1.0, 1.0);

	vec2 normalized_pos = 2.0 * gl_FragCoord.xy/u_screenSize - 1.0;
	float color = iterCount(normalized_pos, u_constant, u_maxIterCount);
	color /= u_maxIterCount;
	gl_FragColor.rgb = lerp(color1, color2, color);
	gl_FragColor.a = 1.0;
}
