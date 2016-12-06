Project: Asteroids
Milestone: In-Class Review
Name: Benjamin Gibson
Class: Software Development I
Instructor: Squirrel Eiserloh

//-----------------------------------------------------------------------------
Known Issues
	None currently aware to me, was able to fix them as I wrote them here.
Build Notes
	No special requirements beyond the normal to note.

	
//-----------------------------------------------------------------------------
How to Use
	Scheme 1: WAD + NumPad
		W - Thrust 
		A, D - Rotate
		NumPad1 - Fire Standard Bullet (also Spacebar as required by spec)
		Numpad2 - Fire Cluster Bullet (fire 5 standard shots to ready)
		Numpad3 - Fire Mega Cluster Bullet (first 10 standard shots to ready)
		Numpad4 - Fire Mega Mega Cluster Bullet (first 15 standard shots to ready)
		P - Reset Ship
	Scheme 2: Arrows + #-Keys
		Up - Thrust
		Left, Right - Rotate
		1 - Fire Standard Bullet (also Spacebar as required by spec)
		2 - Fire Cluster Bullet (fire 5 standard shots to ready)
		3 - Fire Mega Cluster Bullet (first 10 standard shots to ready)
		4 - Fire Mega Mega Cluster Bullet (first 15 standard shots to ready)
		P - Reset Ship
	Scheme 3: XInput GamePad
		Left Stick - Orient Ship
		Right Trigger - Thrust (based on strength of trigger pull)
		A - Fire Standard Bullet 
		B - Fire Cluster Bullet (fire 5 standard shots to ready)
		X - Fire Mega Cluster Bullet (first 10 standard shots to ready)
		Y - Fire Mega Mega Cluster Bullet (first 15 standard shots to ready)
		Start - Reset Ship
	Cheats:
		O - Spawn Asteroid
		L - Destroy Asteroid	
	

//-----------------------------------------------------------------------------
Deep Learning: NOT INSIGHT INTO WHAT WAS LEARNED, BUT INSIGHT INTO HOW DECISIONS WERE MADE
	
		Excessively OverAdhering & UnderAdhering to New Coding Perspectives
	
	Also known as "tabloid programming": I take a newly learned approach to code and run too far with it--like tabloid news. Sometimes adherence to a new dogma caused me no problems: formatting VS2013 to space out parentheses or invoking global constant primitives in place of "magic numbers", both in adherence to readability. On the other hand, I lost hours struggling with understanding specs when the "add-only-as-needed" mindset put me at odds with a Renderer class that only seems necessary in later work. Sometimes even the compiler told me I went too far, as when over-adhering to "minimize #includes in header files" bred several invalid forward declarations. So it can help and harm to be over-adhere, but also under-adhere: I resisted our newest "make it juicy" mindset fearing feature creep would claim me. New but functionally similar bullet types and "growing bossteroids" resulted from a compromise trimming the fat from feature creep through that add-only-as-needed minimalism. It amused me, then, I had the most fun while coding within this tug-of-war pitting features against effort. It yielded a big-picture lens on all the code to spot what I had exposed that begged for reuse in other contexts to form those "juicy" extra features. This led me to form the code for TheGame::SpawnBulletStorm(..) by extracting it from the ship's explosion behaviors. Knowing myself to be easily impressed upon, I do not know that I can resist a teacher's lesson explicitly. Instead, I at least hope to continue to address this newest mindset of "juice" through this "max bang for min buck" style, because I suspect this infectious buzzword, "juice", has a high chance of appearing in future assignments. As great as I agree the attitude to be for Indies like those presented in the videos on the spec, a juice-it attitude makes me feel slightly uncomfortable because I'm not here to learn feature design, or else I'd be an LD. A coder has the ultimate say in a design, but that is a dialogue that I feel should involve coder and designer, and so this just feels like I'm pretending to be both. The only way I can fight that, in my mind, would be to stay as down to earth and as bang for buck as possible.