#include "common.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CSprite;

class Collision{

	glm::vec2 p1, p2, p3, p4, q1, q2, q3, q4;

public:
	Collision();
	void Init(CSprite *sprite1, CSprite *sprite2);
	bool CheckCollision(CSprite *sprite1, CSprite *sprite2);
	void ResolveCollisionPlayer(CSprite *sprite1, CSprite *sprite2);
	void ResolveCollisionEnemy(vector<CSprite*> v1, vector <CSprite*> v2);
	bool getStatusPlayer();
	bool getStatusEnemy();
	virtual ~Collision();
	// Getter pentru Collision
	static Collision * Get() { return &c_me; };

private:
	bool hitPlayer;
	bool hitEnemy;
	static Collision c_me;

};