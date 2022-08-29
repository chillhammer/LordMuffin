#pragma once
#include <Logger/Log.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glad/glad.h>

#ifdef ENABLE_ASSERTS
	#define ASSERT(x, ...) {if (!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

// Allows for commas to be in macros
#define ARGS(...) __VA_ARGS__

#define GLM_ENABLE_EXPERIMENTAL

typedef glm::vec3 Vector3;
typedef glm::vec2 Vector2;
typedef glm::mat4 Matrix4x4;
typedef glm::quat Quaternion;
typedef glm::vec4 Vector4;

typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef long long int64;
typedef int int32;
typedef short int16;
typedef char int8;
typedef int32 bool32;
typedef float float32;
typedef double float64;

// Used to get the running time shared with no regard if it's server or game
#ifdef SERVER
//#define RUNNING_TIME Server.RunningTime()
#define RUNNING_TIME Server.GetFixedFrameDeltaTime() * Server.GetTick()
#else
#define RUNNING_TIME Game.RunningTime()
#endif

#pragma region Debug OpenGl Call
#ifdef SKEL_DEBUG
#define GLCall(x)	\
	GLClearError();	\
	x;				\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__), "OpenGL Error!")
#else
#define GLCall(x) x;
#endif

static void GLClearError()
{
	while (glGetError() != 0);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		LOG_ERROR("[OpenGL Error] ({0}): {1}\n{2}: {3}", error, function, file, line);
		return false;
	}
	return true;
}
#pragma endregion

#define STRING_TO_VECTOR3( str, outVector3 )	\
	{											\
		std::stringstream ss( str );			\
		std::string number;						\
		ss >> number;							\
		outVector3.x = std::stof(number);		\
		ss >> number;							\
		outVector3.y = std::stof(number);		\
		ss >> number;							\
		outVector3.z = std::stof(number);		\
	}											\
	
#define VECTOR3_TO_STRING( vector3, outString )	\
	{											\
		std::stringstream ss;					\
		ss << vector3.x << " " << vector3.y << " " << vector3.z;	\
		outString = ss.str();					\
	}											\

#define DEBUG_BOX( center, halfExtents )		\
	{											\
		Vector3 scale = (2.f * halfExtents);	\
		Vector3 centerVec = center;				\
		centerVec.z += scale.z * 0.5f;			\
		Transform debugTransform;				\
		debugTransform.Position = centerVec;	\
		debugTransform.Scale = scale;			\
		ModelPtr debugBox = Resources.GetModel("WoodenBox");						\
		debugBox->Draw(Resources.GetShader("Debug"), debugTransform.GetMatrix());	\
	}											\

#define DEBUG_SPHERE( center, radius )		\
	{											\
		Vector3 scale = (0.375f * radius);		\
		Vector3 centerVec = center;				\
		Transform debugTransform;				\
		debugTransform.Position = centerVec;	\
		debugTransform.Scale = scale;			\
		ModelPtr debugSphere = Resources.GetModel("Sphere");						\
		debugSphere->Draw(Resources.GetShader("Debug"), debugTransform.GetMatrix());\
	}	