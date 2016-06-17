<?php

header("Content-Type:text/html;charset=utf-8");
ini_set('max_execution_time', 86400 * 30);//设置时间，以免超时中断

/*
More & Original PHP Framwork
Copyright (c) 2007 - 2008 IsMole Inc.
Author: kimi
Documentation: 下载样式文件中的图片，水水专用扒皮工具
from: http://js8.in/586.html
*/
function getCSSImage($file, $pattern)
	//$file = 'images/style.css';
    //$pattern = "/url\((.*)\)/"
	// note 设置PHP超时时间
	set_time_limit(0);

	//note 取得样式文件内容
	$styleFileContent = file_get_contents($file);

	//note 匹配出需要下载的URL地址
	preg_match_all($pattern, $styleFileContent, $imagesURLArray);

	//note 循环需要下载的地址，逐个下载
	$imagesURLArray = array_unique($imagesURLArray[1]);
	foreach($imagesURLArray as $imagesURL) {
		file_put_contents(basename($imagesURL), file_get_contents($imagesURL));
}


function getImage($url,$filename="") {
	if($url=="") return false;

	if($filename=="") {
	$ext=strrchr($url,".");
	if($ext!=".gif" && $ext!=".jpg" && $ext!=".png") return false;
	$filename=date("YmdHis").$ext;
	}

	ob_start();
	readfile($url);
	$img = ob_get_contents();
	ob_end_clean();
	$size = strlen($img);

	$fp2=@fopen($filename, "a");
	fwrite($fp2,$img);
	fclose($fp2);

	return $filename;
} 


$url='http://casarteshoot.sinaapp.com/daochupic';//读取图片地址
$curl=curl_init();//初始化curl

/*设置选项*/
curl_setopt($curl,CURLOPT_URL,$url);
curl_setopt($curl,CURLOPT_RETURNTRANSFER,1);
curl_setopt($curl, CURLOPT_FOLLOWLOCATION, TRUE);
curl_setopt($curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:20.0) Gecko/20100101 Firefox/20.0");

$data=curl_exec($curl);//执行并获取html内容
$data=json_decode($data);//转化数据格式（$url输出的是json格式）
curl_close($curl);//释放句柄
$i=0;
foreach($data as $o){
    $i++;
    $name=$o->uid;
    $time=$o->time;
    getImage($o->url,'./mypic4/'.$name.'_'.$time.'.jpg');//保存到指定文件夹中
}


?>