#ifndef PHYSICAL_OBJECT_H
#define PHYSICAL_OBJECT_H

#include <glm/glm.hpp>
class Collider;
/*======================================================
Physical representation of a game object as a rigid body
======================================================*/
class PhysicalObject
{
private:
	glm::mat3 angularVelocity; // angular velocity of a body
	// freeze the position of object if it has too little energy
	void SleepIfStatic();
public:
	const int type = 0;

	float InvMass; // inverted mass of an object (1 / mass)
	glm::mat3 InvInertia; // inverted inertia tensor ([I]^-1)

	glm::vec3 Position; // position of the center of mass
	glm::mat3 Rotation; // rotation of body
	glm::vec3 Velocity; // linear velocity of the center of mass
	glm::vec3 AngularMomentum; // angular momentum of body

	Collider* BoxCollider; // collider of the body
	/*======================================
	Base constructor of a rigid cuboid body.
	params:
	position[in] - position of the center mass of a body
	a - length of a body (oX)
	b - height of a body (oY)
	c - width of a body (oZ)
	mass - mass of an object (default: -1.0f as an infinitive mass)
	======================================*/
	PhysicalObject(const glm::vec3& position, float a, float b, float c, float mass = -1.0f);
	~PhysicalObject();
	// integrate the effect of <force> force on the body in <point> point in <dt> time
	void ApplyForce(const glm::vec3& force, const glm::vec3& point, float dt);
	// apply <impulse> impulse to <point> point of body
	void ApplyImpulse(const glm::vec3& impulse, const glm::vec3& point);
	// integrate object position and rotation in <dt> time 
	void Move(float dt);
	// move center of mass of an object in <direction> direction in <range> distance
	void Move(const glm::vec3& direction, float range);
	// get the <point> point of body linear velocity (including angle velocity)
	glm::vec3 GetPointVelocity(const glm::vec3& point);
};

#endif
