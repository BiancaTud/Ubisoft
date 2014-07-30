#include "Collision.h"
#include "Sprite.h"

Collision::Collision(){


}

void Collision::Init(CSprite* sprite1, CSprite* sprite2){
	p1.x = p4.x = sprite1->model_mat[3][0];
	p1.y = p2.y = sprite1->model_mat[3][1];
	p2.x = p3.x = p1.x + 0.4f;
	p3.y = p4.y = p1.y + 1.0f;

	q1.x = q4.x = sprite2->model_mat[3][0];
	q1.y = q2.y = sprite2->model_mat[3][1];
	q2.x = q3.x = q1.x + 0.6f;
	q3.y = q4.y = q1.y + 1.0f;

}

bool Collision::CheckCollision(CSprite* sprite1, CSprite* sprite2){

	Init(sprite1, sprite2);
	bool hit = false;
	if ((p1.x > q1.x && p1.x < q2.x && p1.y < q4.y && p1.y > q1.y) ||
		(p2.x > q1.x && p2.x < q2.x && p2.y < q4.y && p2.y > q1.y) ||
		(p3.x > q1.x && p3.x < q2.x && p3.y < q4.y && p3.y > q1.y) ||
		(p4.x > q1.x && p4.x < q2.x && p4.y < q4.y && p4.y > q1.y) ||

		(q1.x > p1.x && q1.x < p2.x && q1.y < p4.y && q1.y > p1.y) ||
		(q2.x > p1.x && q2.x < p2.x && q2.y < p4.y && q2.y > p1.y) ||
		(q3.x > p1.x && q3.x < p2.x && q3.y < p4.y && q3.y > p1.y) ||
		(q4.x > p1.x && q4.x < p2.x && q4.y < p4.y && q4.y > p1.y)){

		hit = true;
		cout << "Coliziuneeeeeeeeeeeeee maximaaaaaaaaaaaaaaaaaaaaaa" << endl;

	}

	return hit;
}

Collision::~Collision()
{

}