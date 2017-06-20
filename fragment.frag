#version 450 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D height;
    float shininess;
    vec3 diffuse_color;
    vec3 specular_color;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define POINT_LIGHTS 100
#define SPOT_LIGHTS 100
#define DIR_LIGHTS 100

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in mat3 TBN;

out vec4 color;

uniform bool UseTexture;
uniform bool UseNormalMap;
uniform bool FillWhite;
uniform bool PerlinNormals;
uniform bool UseCheckers;

uniform float CheckersSize;

uniform vec3 viewPos;
uniform DirLight dirLights[DIR_LIGHTS];
uniform PointLight pointLights[POINT_LIGHTS];
uniform SpotLight spotLights[SPOT_LIGHTS];
uniform Material material;
uniform int PointLightNum;
uniform int SpotLightNum;
uniform int DirLightNum;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 a, vec3 d, vec3 s);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 a, vec3 d, vec3 s);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 a, vec3 d, vec3 s);


vec4 permute(vec4 x){return mod(((x*34.0)+1.0)*x, 289.0);}
vec2 fade(vec2 t) {return t*t*t*(t*(t*6.0-15.0)+10.0);}

float cnoise(vec2 P){
  vec4 Pi = floor(P.xyxy) + vec4(0.0, 0.0, 1.0, 1.0);
  vec4 Pf = fract(P.xyxy) - vec4(0.0, 0.0, 1.0, 1.0);
  Pi = mod(Pi, 289.0);
  vec4 ix = Pi.xzxz;
  vec4 iy = Pi.yyww;
  vec4 fx = Pf.xzxz;
  vec4 fy = Pf.yyww;
  vec4 i = permute(permute(ix) + iy);
  vec4 gx = 2.0 * fract(i * 0.0243902439) - 1.0;
  vec4 gy = abs(gx) - 0.5;
  vec4 tx = floor(gx + 0.5);
  gx = gx - tx;
  vec2 g00 = vec2(gx.x,gy.x);
  vec2 g10 = vec2(gx.y,gy.y);
  vec2 g01 = vec2(gx.z,gy.z);
  vec2 g11 = vec2(gx.w,gy.w);
  vec4 norm = 1.79284291400159 - 0.85373472095314 *
    vec4(dot(g00, g00), dot(g01, g01), dot(g10, g10), dot(g11, g11));
  g00 *= norm.x;
  g01 *= norm.y;
  g10 *= norm.z;
  g11 *= norm.w;
  float n00 = dot(g00, vec2(fx.x, fy.x));
  float n10 = dot(g10, vec2(fx.y, fy.y));
  float n01 = dot(g01, vec2(fx.z, fy.z));
  float n11 = dot(g11, vec2(fx.w, fy.w));
  vec2 fade_xy = fade(Pf.xy);
  vec2 n_x = mix(vec2(n00, n01), vec2(n10, n11), fade_xy.x);
  float n_xy = mix(n_x.x, n_x.y, fade_xy.y);
  return 2.3 * n_xy;
}

void main() {
    if (FillWhite) {
        color = vec4(1.0);
        return;
    }
    vec3 norm = normalize(Normal);
    if (UseNormalMap) {
        norm = texture(material.normal, TexCoords).rgb;
        norm = normalize(norm * 2.0 - 1.0);
        norm = normalize(TBN * norm);
    }
    if (PerlinNormals) {
        const float mult = 32;
        const float scale = 0.25;
        const float offset = 1;
        vec3 n = vec3(cnoise((TexCoords - vec2(offset, 0)) * mult) -
                        cnoise((TexCoords + vec2(offset, 0)) * mult),
                        cnoise((TexCoords - vec2(0, offset)) * mult) -
                        cnoise((TexCoords + vec2(0, offset)) * mult), 2 * scale);
        n = normalize(n);
        norm = normalize(TBN * n);
    }
    vec3 ambient = vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = vec3(texture(material.diffuse, TexCoords));
    vec3 specular = vec3(texture(material.specular, TexCoords));
    if (!UseTexture) {
        ambient = material.diffuse_color;
        diffuse = material.diffuse_color;
        specular = material.specular_color;
    }
    //shashechka
    if (UseCheckers) {
        if ((int(TexCoords.x / CheckersSize) + int(TexCoords.y / CheckersSize)) % 2 == 1) {
            ambient = vec3(1.0);
            diffuse = vec3(1.0);
            specular = vec3(1.0);
        }
        else {
            ambient = vec3(0.1);
            diffuse = vec3(0.1);
            specular = vec3(0.1);
        }
    }
    //shashechka
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0);
    for(int i = 0; i < DirLightNum; i++) {
            result += CalcDirLight(dirLights[i], norm, viewDir, ambient, diffuse, specular);
        }
    for(int i = 0; i < PointLightNum; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, ambient, diffuse, specular);
    }
    for(int i = 0; i < SpotLightNum; i++) {
            result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir, ambient, diffuse, specular);
    }
    color = vec4(result, 1.0);
//    color = vec4((norm + 1) / 2.0, 1.0);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 a, vec3 d, vec3 s) {
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 ambient = light.ambient * a;
    vec3 diffuse = light.diffuse * diff * d;
    vec3 specular = light.specular * spec * s;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 a, vec3 d, vec3 s) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec3 ambient = light.ambient * a;
    vec3 diffuse = light.diffuse * diff * d;
    vec3 specular = light.specular * spec * s;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 a, vec3 d, vec3 s) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    vec3 ambient = light.ambient * a;
    vec3 diffuse = light.diffuse * diff * d;
    vec3 specular = light.specular * spec * s;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}