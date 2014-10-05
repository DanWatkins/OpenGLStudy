#version 430 core

layout (location = 0) in vec4 inPositionMass;
layout (location = 1) in vec3 inVelocity;
layout (location = 2) in ivec4 inConnections;

layout (binding = 2) uniform samplerBuffer texPosition;

out vec4 tfPositionMass;
out vec3 tfVelocity;

uniform float timeDelta = 0.7;
uniform float k = 7.1;

const vec3 gravity = vec3(0.0, -0.08, 0.0);

uniform float damping = 2.8;
uniform float restLength = 0.88;

void main()
{
	vec3 position = inPositionMass.xyz;
	float mass = inPositionMass.w;
	vec3 velocity = inVelocity;
	vec3 force = gravity*mass - damping*velocity;
	bool fixedNode = true;

	for (int i=0; i<4; i++)
	{
		if (inConnections[i] != -1)
		{
			//q is the position of the other vertex
			vec3 q = texelFetch(texPosition, inConnections[i]).xyz;
			vec3 d = q-position;

			float x = length(d);
			force += -k * (restLength - x) * normalize(d);
			fixedNode = false;
		}
	}

	//if this is a fixed node, reset the force to zero
	if (fixedNode)
	{
		force = vec3(0.0);
	}

	//acceleration due to force
	vec3 acceleration = force / mass;

	//displacement
	vec3 displacement = velocity*timeDelta + 0.5*acceleration*timeDelta*timeDelta;

	//final velocity
	vec3 finalVelocity = velocity + acceleration*timeDelta;

	//constrain the absolute value of the displacement per step
	displacement = clamp(displacement, vec3(-25.0), vec3(25.0));

	//write the outputs
	tfPositionMass = vec4(position+displacement, mass);
	tfVelocity = finalVelocity;
}