<?php
/*
More & Original PHP Framwork
Copyright (c) 2007 - 2008 IsMole Inc.
Author: kimi
Documentation: 下载样式文件中的图片，水水专用扒皮工具
from: http://js8.in/586.html
*/

header("Content-Type:text/html;charset=utf-8");
ini_set('max_execution_time', 86400 * 30);//设置时间，以免超时中断


function getCSSImage($file, $pattern)
    //$file = 'images/style.css';
    //$pattern = "/url\((.*)\)/"
    set_time_limit(0);   //设置PHP超时时间

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

?>
