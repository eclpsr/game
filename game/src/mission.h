#ifndef MISSION_H_
#define MISSION_H_

int getCurrentMission(int x){
	int mission = 0;
if((x>0) && (x<600)) { mission = 0; }
if (x>400) { mission = 1; }
if (x>700) { mission = 2; }
if (x>2200) { mission = 3; }
return mission;
}

std::string getTextMission(int currentMission) {
	std::string missionText = "";

	switch (currentMission) {
	case 0: missionText = "\nMission0\nbegin"; break;
	case 1: missionText = "\nMission1\nbegin"; break;
	case 2: missionText = "\nMission2\nbegin"; break;
	case 3: missionText = "\nMission3\nbegin"; break;
	}
return missionText;
};



#endif /* MISSION_H_ */
