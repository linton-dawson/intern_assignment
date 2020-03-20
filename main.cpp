#include <stdio.h>
#include<thread>
#include<chrono>
#include <opencv2/opencv.hpp>
#include<filesystem>
#include<functional>
#include<fstream>

using namespace cv;

//Function to get filename
std::string getLabel()
{
	std::string basicPath = "./filter_vids/";
	const auto current_time = std::chrono::system_clock::now();
	std::string time_as_string = std::to_string(std::chrono::duration_cast<std::chrono::seconds>(current_time.time_since_epoch()).count());
	basicPath += time_as_string;
	return basicPath;
}


//Function to authenticate username and password
bool logAuth ( std::string uPwd )
{
	//Hash matching
	std::hash<std::string > ipHash;
	std::string hashVal = std::to_string(ipHash(uPwd));
	std::string str;

	std::ofstream ofile("usrpwd.txt", std::ofstream::out | std::ofstream::trunc);
	std::hash <std::string> pwHash;
	std::string pwdStr =std::to_string(pwHash("tooroduszaphod42"));
	ofile<<pwdStr;
	ofile.close();

	std::ifstream pwdFile("usrpwd.txt");
	if(pwdFile.is_open()) {
		while(std::getline(pwdFile,str) ) {
			if (str == hashVal){
				pwdFile.close();
				return true;
			}
		}
	}

	std::cerr<<"Incorrect password !\n";
	pwdFile.close();
	return false;
}


//Function to check 10 second time interval
const bool endVideo(std::chrono::system_clock::time_point start) {
	auto end = std::chrono::system_clock::now();
	auto diff = end - start;
	if(diff.count() >= 1e10)
		return true;
	return false;
}


int main()
{
	std::string uName, uPwd ;
	std::cout<<"Enter username : ";
	std::cin>>uName;
	std::cout<<"Enter password : ";
	std::cin>>uPwd;

	//Authenticating username and password
	if(logAuth(uName + uPwd)) {
		std::cout<<"Webcam will now be starting ...\n";
		std::this_thread::sleep_for(std::chrono::seconds(2));
		Mat frame;
		VideoCapture vcap(0);
		
		
		if(!vcap.isOpened()) {
			std::cerr<< "Can't open video camera\n";
			return -1;
		}

		vcap>>frame;
	
		VideoWriter vwrite;
	
		// Getting primitive data type
		bool isColor = (frame.type() == CV_8UC1);
	
		// Using FMP4 codec @ 10fps
		int codec = VideoWriter::fourcc('F','M','P','4');
	
		double fps = 10.0;
	
		// Setting filename
		std::string fname = getLabel();
		fname += ".avi";
	
		//Assigning attributes to video file
		vwrite.open(fname,codec,fps,frame.size(),isColor);
		
		//Intializing clock to measure 10 seconds
		auto start = std::chrono::system_clock::now();
		std::chrono::system_clock::time_point end;
			
	
		std::cout<<"Recording video for 10 seconds. Please wait...\n";
		for(;;)
		{
			vcap.read(frame);
			if(frame.empty()) {
				std::cerr<<"EMPTY FRAME !\n";
				break;
			}
	
			//Splitting frames in RGB channels respectively
			Mat rgbc[3];
			split(frame,rgbc);
	
			//Display normal frames
			namedWindow("Active Window",WINDOW_NORMAL);
	
			//Fullscreen mode
	//		setWindowProperty("Active Window", WND_PROP_FULLSCREEN, 1);
		
				imshow("Active Window",frame);
	
			//Performing Gaussian Blur on red channel using 17x17 Gaussian kernel
			Mat gaussFrame;
			GaussianBlur(rgbc[0],gaussFrame,Size(17,17),0);
			vwrite.write(gaussFrame);
			
			//break if any key pressed
			if(waitKey(5) >= 0)
				break;
	
			//Break if 10 seconds completed	
			if(endVideo(start)) {
				std::cout<<"Recording done successfully !\nFile stored in "<<fname<<std::endl;
				break;
			}
			
		}
	}
	
	//File display code
	std::cout<<"\nFiles in ./filter_vids are : \n";
	std::string path = "./filter_vids";
	for( const auto & entry : std::filesystem::directory_iterator(path))
		std::cout<<entry.path()<<std::endl;

	return 0;
}

