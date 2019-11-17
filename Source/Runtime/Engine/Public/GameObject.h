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

	Mesh &getMesh() { return _mesh; }

private:
	Mesh _mesh;
};