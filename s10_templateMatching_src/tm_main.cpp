#include "tm_main.h"

void GenMatchMap(cv::Mat srcM, cv::Mat tempM, cv::Mat &mapM, int &x, int &y){
	int i, j, m, n, inp_x, inp_y, tmp_x, tmp_y;
	double max = -5000, mnsumit, sumi, sumt, mnsumi2, mnsumt2;
	cv::Mat gM, gtmpM, dmapM;
	
	// 画像サイズをコピー
	inp_x = srcM.cols; inp_y = srcM.rows;
	tmp_x = tempM.cols; tmp_y = tempM.rows;
	
	// 入力画像のチャンネル数にあわせてグレー表示用のメモリへコピーする
	if(srcM.channels() == 1){
		// そのままをコピー（つまり、読み込んだ画像がもとからグレーである）
		gM = srcM.clone();
	}else{
		// RGBからグレースケール化してコピー（読み込んだ画像がカラーだった場合）
		gM = cv::Mat::zeros(srcM.size(), CV_8UC1);
		cv::cvtColor(srcM, gM, cv::COLOR_BGR2GRAY);
	}

	// 入力画像のチャンネル数にあわせてグレー表示用のメモリへコピーする
	if(tempM.channels() == 1){
		// そのままをコピー（つまり、読み込んだ画像がもとからグレーである）
		gtmpM = tempM.clone();
	}else{
		// RGBからグレースケール化してコピー（読み込んだ画像がカラーだった場合）
		gtmpM = cv::Mat::zeros(tempM.size(), CV_8UC1);
		cv::cvtColor(tempM, gtmpM, cv::COLOR_BGR2GRAY);
	}
	printf("%d, %d - %d, %d\n", inp_x, inp_y, tmp_x, tmp_y);
	
	dmapM = cv::Mat::zeros(srcM.size(), CV_64FC1);
	
	// マップの生成
	for(j = 0; j < inp_y; j++){
		for(i = 0; i < inp_x; i++){
			if(i + tmp_x < inp_x && j + tmp_y < inp_y){
				mnsumit = sumi = sumt = mnsumi2 = mnsumt2 = 0;
				for(n = 0; n < tmp_y; n++){
					for(m = 0; m < tmp_x; m++){
						mnsumit += gM.at<uchar>(j + n, i + m) * gtmpM.at<uchar>(n, m);
						sumi += gM.at<uchar>(j + n, i + m);
						sumt += gtmpM.at<uchar>(n, m);
						mnsumi2 += gM.at<uchar>(j + n, i + m) * gM.at<uchar>(j + n, i + m);
						mnsumt2 += gtmpM.at<uchar>(n, m) * gtmpM.at<uchar>(n, m);
					}
				}
				// Cross correlationを1パスで計算する式 (の簡略版)
				dmapM.at<double>(j, i) = (mnsumit - sumi * sumt) / sqrt((mnsumi2 - sumi * sumi) * (mnsumt2 - sumt * sumt));
				if(max < dmapM.at<double>(j, i)){
					max = dmapM.at<double>(j, i);
					x = i;
					y = j;
				}
//				printf("%f, %f, %f, %f, %f, %f, %f\n", (double)inp[j * inp_x + i], mnsumit, sumi, sumt, mnsumi2, mnsumt2, map[j * inp_x + i]);
			}
		}
	}
	printf("(%d, %d) %f\n", x, y, max);
	dmapM.convertTo(mapM, CV_8UC1, 200);
}
