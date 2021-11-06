#version 330 core

out vec4 fragColor;
in vec2 vertexPosition;

uniform vec2 resolution;
uniform vec2 position;

float draw_circle(float radius, vec2 center, vec2 uv) {
    float d = distance(center, uv);
    return 1.0 - smoothstep(radius - 1.0 / resolution.y, radius + 1.0 / resolution.y, d);
}

void main() {
    vec2 uv = (gl_FragCoord.xy + (vertexPosition.xy - 0.5) * resolution.xy) / resolution.xy;
    float radius = 0.05;
    vec2 center = position.xy * 0.189;
    float circle = draw_circle(radius, center, uv);
    if (circle < 1.0)
    {
        discard;
    }
    fragColor = vec4(1.0, 0.5, 0.2, 1.0);
}