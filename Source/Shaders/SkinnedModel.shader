#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in ivec4 BoneIDs;
layout(location = 4) in vec4 Weights;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragPos;

const int MAX_BONES = 100;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;
uniform mat4 u_Bones[MAX_BONES];

void main()
{
	mat4 boneTransform = u_Bones[BoneIDs[0]] * Weights[0];
	boneTransform += u_Bones[BoneIDs[1]] * Weights[1];
	boneTransform += u_Bones[BoneIDs[2]] * Weights[2];
	boneTransform += u_Bones[BoneIDs[3]] * Weights[3];

	gl_Position = u_ViewProjection * u_Model * boneTransform * vec4(position, 1.0);
	v_FragPos = vec3(u_Model * vec4(position, 1.0));
	v_TexCoord = texCoord;
	v_Normal = (boneTransform * vec4(normals, 0.0)).xyz;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

uniform vec3 u_Color;
uniform sampler2D u_Texture;
uniform bool u_UseTexture;
uniform vec3 u_LightPosition;

void main()
{
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_LightPosition - v_FragPos);
	float intensity = dot(norm, lightDir);

	// Cell-Shading
	intensity = (intensity + 0.3f > 0.5f ? 0.8f : 0.2f);
	float baseline = 0.6f;


	if (u_UseTexture)
		color = texture(u_Texture, v_TexCoord);
	else
	{
		color = vec4(u_Color, 1.0);
	}

	color *= (baseline + intensity);
	color.w = 1.0f;
};