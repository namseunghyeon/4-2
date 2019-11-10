#pragma once

class GameObject : public Transform
{
public:
	FORCEINLINE GameObject(const Transform& inTransform = Transform())
	{
		Transform::Transform(inTransform);
		/*SetPosition(inTransform.getPosition());
		SetRotate(inTransform.getRotate());
		SetScale(inTransform.getScale());*/
	}

private:
};