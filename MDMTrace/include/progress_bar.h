#include <iostream>

void progress_bar(long long i, long long max)
{
	float progress = (double)i / (double) max;
	int barWidth = 70;

	if(i == 0) { std::cout << "["; };
	int pos = barWidth * progress;
	for (int i = 0; i < barWidth; ++i) {
		if (i < pos) std::cout << "=";
		else if (i == pos) std::cout << ">";
		else std::cout << " ";
	}
	std::cout << "] " << int(progress * 100.0) << " %\r";
	std::cout.flush();

	if(i == max-1) { 
		std::cout << "\nDone!" << std::endl;
	}
}

