
class Timing
{
public:

	Timing();
	
	void Update();

	float GetDeltaTime() const { return mDeltaTime; }

	double GetTime() const;

	inline float GetTimef() const
	{
		return static_cast< float >( GetTime() );
	}


	static Timing sInstance; 

private:
	float		mDeltaTime;
	uint64_t	mDeltaTick;

	double		mLastFrameStartTime;
	double		mPerfCountDuration;

};