#include "InGameScene.hpp"

InGameScene::InGameScene()
:
question({ Vec2(340, 705), Vec2(390, 650), Vec2(390, 760), Vec2(1530, 650), Vec2(1530, 760), Vec2(1580, 705) }),
answer_a({ Vec2(340, 845), Vec2(390, 790), Vec2(390, 900), Vec2(896, 790), Vec2(896, 900), Vec2(946, 845) }),
answer_b({ Vec2(974, 845), Vec2(1024, 790), Vec2(1024, 900), Vec2(1530, 790), Vec2(1530, 900), Vec2(1580, 845) }),
answer_c({ Vec2(340, 985), Vec2(390, 930), Vec2(390, 1040), Vec2(896, 930), Vec2(896, 1040), Vec2(946, 985) }),
answer_d({ Vec2(974, 985), Vec2(1024, 930), Vec2(1024, 1040), Vec2(1530, 930), Vec2(1530, 1040), Vec2(1580, 985) })
{
	double area_1 = Hexa::CalculateHexagonArea(answer_a._Vertices());
	double area_2 = Hexa::CalculateHexagonArea(answer_b._Vertices());
	double area_3 = Hexa::CalculateHexagonArea(answer_c._Vertices());
	double area_4 = Hexa::CalculateHexagonArea(answer_d._Vertices());

	bool equality = area_1 == area_2 && area_2 == area_3 && area_3 == area_4;

	printf("%f\n", area_1);
	printf("%f\n", area_2);
	printf("%f\n", area_3);
	printf("%f\n", area_4);

	if(equality == false)
	{
		throw runtime_error(format("The 4 answer textboxes do not have equal size.\n1.) {0}\n2.) {1}\n3.) {2}\n4.) {3}", area_1, area_2, area_3, area_4));
	}
}

void InGameScene::Draw() const
{
	question.Draw();
	answer_a.Draw();
	answer_b.Draw();
	answer_c.Draw();
	answer_d.Draw();
}