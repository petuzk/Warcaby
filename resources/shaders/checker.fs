#version 330

#define MAX_LIGHTS       10
#define LIGHT_AMBIENT    0
#define LIGHT_POINT      1

struct Light {
    int enabled;
    int type;
    vec3 position;
    vec4 color;
};



// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
in vec3 fragNormal;

uniform Light lights[MAX_LIGHTS];
uniform vec3 viewPos;
uniform vec4 colDiffuse;

uniform float shine;     // 16   [1-128]
uniform float addBase;   // 0.4  [0-1]
uniform float kSpecular; // 0.4  [0-1]
uniform float dAmbient;  // 10   [1-30]
uniform float gammaR;    // 1.6  [0.5-2.5]
uniform float gammaG;    // 1.6  [0.5-2.5]
uniform float gammaB;    // 1.6  [0.5-2.5]

// Output fragment color
out vec4 finalColor;



void main() {
    vec4 texelColor = colDiffuse;
    vec3 lightDot = vec3(0.0);
    vec3 normal = fragNormal;
    vec3 viewD = normalize(viewPos - fragPosition);
    vec3 specular = vec3(0.0);
    vec4 ambient = vec4(0.0);

    for (int i = 0; i < MAX_LIGHTS; i++) {
        if (lights[i].enabled == 1) {
            if (lights[i].type == LIGHT_AMBIENT) {
                ambient = lights[i].color;
            } else {
                vec3 light = normalize(lights[i].position - fragPosition);

                float NdotL = max(dot(normal, light), 0.0);
                lightDot += lights[i].color.rgb*NdotL;

                float specCo = 0.0;
                if (NdotL > 0.0) specCo = pow(max(0.0, dot(viewD, reflect(-(light), normal))), shine);
                specular += specCo;
            }
        }
    }

    finalColor = (
        texelColor*(
            (
                vec4(vec3(addBase), 1.0) + vec4(specular * kSpecular, 1.0)
            )*vec4(lightDot, 1.0)
        )
    );

    finalColor += texelColor*(ambient/dAmbient);

    // Gamma correction
    finalColor = pow(finalColor, vec4(1.0/vec3(gammaR, gammaG, gammaB), 1.0));
}
