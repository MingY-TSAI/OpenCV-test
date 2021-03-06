#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/objdetect.hpp>
#include<iostream>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

using namespace cv;
using namespace std;


string OCR(char path[])
{
	char* outText;

	tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
	// Initialize tesseract-ocr with English, without specifying tessdata path
	if (api->Init(NULL, "eng")) {
		cout << "Could not initialize tesseract.\n";
		exit(1);
	}

	// Open input image with leptonica library
	Pix* image = pixRead(path);
	api->SetImage(image);
	// Get OCR result
	outText = api->GetUTF8Text();


	// Destroy used object and release memory
	api->End();
	delete api;
	//delete[] outText;
	pixDestroy(&image);

	return outText;
}




void main()
{
	VideoCapture cap(0);
	Mat img;
	
	CascadeClassifier plateCasecade;
	plateCasecade.load("Resources/haarcascade_russian_plate_number.xml");

	if (plateCasecade.empty()) { cout << "XML file  not loaded"; }

	vector<Rect> plates;

	while (true) {
		cap.read(img);

		plateCasecade.detectMultiScale(img, plates, 1.1, 10);

		for (int i = 0; i < plates.size(); i++)
		{
			Mat imgCrop = img(plates[i]);
			//imshow(to_string(i), imgCrop);                                                                                                                       
			

			string tmp_string = "Resources/plates/" + to_string(i) + ".png";
			char *path = &tmp_string[0];

			imwrite(tmp_string ,imgCrop);
			rectangle(img, plates[i].tl(), plates[i].br(), Scalar(255, 0, 255), 3);

			string ocr_string = OCR(path);
			cout << "OCR¿é¥X: " << ocr_string << endl;
		}

		imshow("Image", img);

	waitKey(1);
	}
}


