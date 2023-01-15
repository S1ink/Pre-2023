#include "cpp-tools/src/sighandle.h"
#include "cpp-tools/src/timing.h"

#include <core/visionserver2.h>
#include <core/visioncamera.h>
#define APRILPOSE_DEBUG
#include <core/aprilpose.h>
#include <core/config.h>

#include <vector>

#include "calibrations.h"
#include "field.h"


StopWatch runtime("Runtime", &std::cout, 0);
void on_exit() {
	runtime.end();
}

int main(int argc, char** argv) {
    runtime.setStart();
	SigHandle::get();
	atexit(on_exit);

    std::vector<VisionCamera> cameras;
    cv::Ptr<cv::aruco::DetectorParameters> aparams = cv::aruco::DetectorParameters::create();
    //aparams->cornerRefinementMethod = cv::aruco::CORNER_REFINE_APRILTAG;

    if(argc > 1 && initNT(argv[1]) && createCameras(cameras, calibrations, argv[1])) {}
	else if(initNT() && createCameras(cameras, calibrations)) {}
	else { return EXIT_FAILURE; }

    vs2::VisionServer::Init();
    vs2::VisionServer::addCameras(std::move(cameras));
    vs2::VisionServer::addStreams(1);
    AprilPose ap{FIELD_2023, aparams};
    vs2::VisionServer::addPipeline(&ap);
    vs2::VisionServer::compensate();
    vs2::VisionServer::run(60.f);
    atexit(vs2::VisionServer::stopExit);


}