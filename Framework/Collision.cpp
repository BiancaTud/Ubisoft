#include "Collision.h"
#include "Sprite.h"
#include "SpriteManager.h"

Collision::Collision(){
	hitPlayer = false;
	hitEnemy = false;
}

void Collision::Init(CSprite* sprite1, CSprite* sprite2){
	p1.x = p4.x = sprite1->model_mat[3][0];
	p1.y = p2.y = sprite1->model_mat[3][1];
	if (sprite1->type == 0){
		p2.x = p3.x = p1.x + 0.4f;
		p3.y = p4.y = p1.y + 1.0f;
	}
	else if(sprite1->type==-1){
		p2.x = p3.x = p1.x + 0.4f/3;
		p3.y = p4.y = p1.y + 1.0f/3;

	}

	q1.x = q4.x = sprite2->model_mat[3][0];
	q1.y = q2.y = sprite2->model_mat[3][1];
	q2.x = q3.x = q1.x + 0.3f;
	q3.y = q4.y = q1.y + 0.9f;

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


void Collision::ResolveCollision(CSprite *sprite1, CSprite *sprite2){


    //coliziune intre player si un inamic
	if (sprite1->type == 0){
		bool check = CheckCollision(sprite1, sprite2);

		if (check == true){
			sprite1->SetPosition(glm::vec3(sprite1->GetPosition().x,
				sprite1->GetPosition().y - 4.0f, sprite1->GetPosition().z));
			check = false;
		}
	}

	
	else if (sprite1->type == -1){




	}



}

Collision::~Collision()
{

}