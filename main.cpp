/***********************************************************************************/
/*          OneCut - software for interactive image segmentation                   */
/*          "Grabcut in One Cut"                                                   */
/*          Meng Tang, Lena Gorelick, Olga Veksler, Yuri Boykov,                   */
/*          In IEEE International Conference on Computer Vision (ICCV), 2013       */
/*          https://github.com/meng-tang/OneCut                                    */
/*          Contact Author: Meng Tang (mtang73@uwo.ca)                             */
/***********************************************************************************/

#include "OneCut.h"
#include "myutil.h"
#include <iostream>

int main(int argc, char * argv[])
{
	// set default parameters
	int colorBinSize = 8; // size of color bin
	int gridConnectivity = 8; // 4, 8 or 16 connect Grid
  double weightPotts = 9.0; // weight of Potts term
	MAXFLOW maxflowoption = IBFS; // either use BK or IBFS algorithm. BK is NOT recommended here.
  
  // required parameters
  const char *inputFileName, *bboxFileName, *outputFileName;

	int i = argc-1;
  outs(i)
	while(i > 0)
	{
		std::string temp(argv[i]);
    outs('temp')
		if(temp == "-i" || temp == "--input")
		{
  		inputFileName = argv[i+1];
		}
		else if(temp == "-b" || temp == "--bbox")
		{
      bboxFileName = argv[i+1];
		}
		else if(temp == "-o" || temp == "--output")
		{
      outputFileName = argv[i+1];
		}
		else if(temp == "-p" || temp == "--potts")
		{
  		weightPotts = atof( argv[i+1] );
		}
		else if(temp == "--test")
		{
      inputFileName = "./images/sub1.40-1%.bmp";
      bboxFileName = "./images/sub1.40-1%_box.bmp";
      outputFileName = "./images/sub1.40-1%_result.bmp";
		}
		else if(temp == "-h" || temp == "--help")
    {
			std::cout << "-i or --input INPUT_FILE, -b or --bbox BOUNDINGBOX_FILE, -o or --output OUTPUT_FILE, -p or --potts WEIGHT_POTTS, -h or --help (displays this message)" << std::endl;
			exit(EXIT_SUCCESS);
    }
		i--;
	}
  outs(inputFileName)
  
	outs("load input image");
	Table2D<RGB> image = loadImage<RGB>(inputFileName);
	clock_t start = clock(); // Timing
	OneCut onecut(image, colorBinSize, gridConnectivity, maxflowoption); // 8 connect 32 bins per channel
	onecut.print();
	
	outs("load bounding box");
	Table2D<int> box = loadImage<RGB>(bboxFileName);
	
	onecut.constructbkgraph(box, weightPotts);

	outs("run maxflow/mincut");
	Table2D<Label> segmentation = onecut.run();

	outs("save segmentation");
	savebinarylabeling(image, segmentation, outputFileName);

	// timing
	cout<<"\nIt takes "<<(double)(clock()-start)/CLOCKS_PER_SEC<<" seconds!"<<endl;

	return -1;
}