#pragma once


//-----------------------------------------------------------------------------
class Entity;


//-----------------------------------------------------------------------------
class Bullet : public Entity
{
public:
	enum BulletType { STANDARD = 0, CLUSTER = 1, MEGA_CLUSTER = 2, MEGA_MEGA_CLUSTER = 3 };
	static const int CLUSTER_BULLET_STORM_SIZE = 50;
	static const int NUMBER_SHOTS_BEFORE_CLUSTER_READY = 5; //So low for the sake of speeding up the demo.
	Bullet();
	Bullet( const Vector2& position, const float m_orientation, const BulletType type = STANDARD);
	float GetTimeToLive() const { return m_timeToLive; }
	BulletType GetBulletType() const { return m_type; }
	void Render();
	void Update( float deltaSeconds ) override;
private:
	BulletType m_type;
	float m_timeToLive;
};