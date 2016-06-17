<?php

header("Content-Type:text/html;charset=utf-8");
ini_set('max_execution_time', 86400 * 30);//����ʱ�䣬���ⳬʱ�ж�

/*
More & Original PHP Framwork
Copyright (c) 2007 - 2008 IsMole Inc.
Author: kimi
Documentation: ������ʽ�ļ��е�ͼƬ��ˮˮר�ð�Ƥ����
from: http://js8.in/586.html
*/
function getCSSImage($file, $pattern)
	//$file = 'images/style.css';
    //$pattern = "/url\((.*)\)/"
	// note ����PHP��ʱʱ��
	set_time_limit(0);

	//note ȡ����ʽ�ļ�����
	$styleFileContent = file_get_contents($file);

	//note ƥ�����Ҫ���ص�URL��ַ
	preg_match_all($pattern, $styleFileContent, $imagesURLArray);

	//note ѭ����Ҫ���صĵ�ַ���������
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


$url='http://casarteshoot.sinaapp.com/daochupic';//��ȡͼƬ��ַ
$curl=curl_init();//��ʼ��curl

/*����ѡ��*/
curl_setopt($curl,CURLOPT_URL,$url);
curl_setopt($curl,CURLOPT_RETURNTRANSFER,1);
curl_setopt($curl, CURLOPT_FOLLOWLOCATION, TRUE);
curl_setopt($curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:20.0) Gecko/20100101 Firefox/20.0");

$data=curl_exec($curl);//ִ�в���ȡhtml����
$data=json_decode($data);//ת�����ݸ�ʽ��$url�������json��ʽ��
curl_close($curl);//�ͷž��
$i=0;
foreach($data as $o){
    $i++;
    $name=$o->uid;
    $time=$o->time;
    getImage($o->url,'./mypic4/'.$name.'_'.$time.'.jpg');//���浽ָ���ļ�����
}


?>