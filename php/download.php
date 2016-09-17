<?php
/**
@author: denny
@date: 2011-8-23
@origin function: curl, socket, fopen/fread, fsockopen/fwrite, file_get_contents
@function: request_by_fileio/request_by_socket/request_by_curl/request_by_file/request_by_file2
@library:
    simple_html_dom.php
    Snoopy.class.php  :fetch() fetchtext() fetchlinks()
@note:
**/

ini_set("max_execution_time", 2);

$url = "http://www.phpzixue.cn";

/**
 * request_by_fileio~~
 * @called: fopen->fread->fclose
 * @example:
    $post_string = "app=socket&version=beta";
    request_by_socket('facebook.cn','/restServer.php', $post_string);
 */
function request_by_fileio($url)
{
    $handle = fopen ($url, "rb");
    $contents = "";
    do {
        $data = fread($handle, 1024);
        if (strlen($data) == 0) {
            break;
        }
        $contents .= $data;
    } while (true);
    fclose ($handle);
    return $contents;
}

/**
 * request_by_socket~~Socket版本
 * @called: fsockopen
 * @example:
    $post_string = "app=socket&version=beta";
    request_by_socket('facebook.cn','/restServer.php', $post_string);
 */
function request_by_socket($url, $remote_path, $post_string, $port = 80, $timeout = 30)
{
    $socket = fsockopen($url, $port, $errno, $errstr, $timeout);
    if (!$socket) die("$errstr($errno)");

    fwrite($socket, "POST $remote_path HTTP/1.0\r\n");
    fwrite($socket, "User-Agent: Socket Example\r\n");
    fwrite($socket, "HOST: $url\r\n");
    fwrite($socket, "Content-type: application/x-www-form-urlencoded\r\n");
    fwrite($socket, "Content-length: " . (strlen($post_string) + 8) . '\r\n');
    fwrite($socket, "Accept:*/*\r\n");
    fwrite($socket, "\r\n");
    if ($post_string != '' ){
        fwrite($socket, "mypost=$post_string\r\n");
    }
    fwrite($socket, "\r\n");
    $header = "";
    while ($str = trim(fgets($socket, 4096))) {
        $header .= $str;
    }
    $data = "";
    while (!feof($socket)) {
        $data .= fgets($socket, 4096);
    }
    return $data;
}


/**
 * request_by_curl~~Curl版本
 * @example:
    $post_string = "app=request&version=beta";
    request_by_curl('http://facebook.cn/restServer.php', $post_string);
 * http_build_query(ARRAY)
 */
function request_by_curl($url, $post_string)
{
    // making string from $data: urlencode, string -> http_build_query(ARRAY)
    //$posts = "";
    //foreach($post_data as $key=>$value)
       // $posts.="$key=".urlencode($value)."&";

    $ch = curl_init();
    //curl_setopt($ch, CURLOPT_POST, 1);
    //curl_setopt($ch, CURLOPT_HEADER, 0);
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $post_string);  // CURLOPT_POSTFIELDS
    //curl_setopt($curl, CURLOPT_POSTFIELDS, http_build_query($post));  //$post=array
    //为了支持cookie
    curl_setopt($ch, CURLOPT_COOKIEJAR, 'cookie.txt');
    //curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    $data = curl_exec($ch);
    curl_close($ch);
    return $data;
}


/**
 * request_by_file
 * @called: file_get_contents
 * @example:
    $post_string = "app=request&version=beta";
    request_by_file('http://facebook.cn/restServer.php', $post_string);
 */
function request_by_file($url, $post_string)
{
    $context = array(
        'http' => array(
            'method' => 'POST',
            'header' => 'Content-type: application/x-www-form-urlencoded' .
                        '\r\n'.'User-Agent : Jimmy\'s POST Example beta' .
                        '\r\n'.'Content-length:' . strlen($post_string) + 8,
            'content' => 'mypost=' . $post_string)
        );
    $stream_context = stream_context_create($context);
    $data = file_get_contents($url, false, $stream_context);
    return $data;
}

/**
* request_by_file2
* @param
    $url 可以是带参数的url
    $timeout 设置超时
    $params 参数字符串,如a=2&b=3,通常用http_build_query生成
    $method 'GET','POST','' 为空等表示不对参数处理
    $times
* @called:  file_get_contents
* @return string|boolean
**/
function request_by_file2($url = '', $timeout = 10, $params = '', $method = 'GET', $times = 3) {
    //提交方式
    $method =   strtoupper($method);
    //基本选项
    $opts['http'] = array(
                        'method'=>$method,
                        'timeout'=>$timeout,//设置超时时间(秒)
                    );

    //判断参数提交方式
    if(!empty($params)) {
        if('POST' == $method) {
            $opts['http']['content']    =   $params;
        }
        elseif('GET' == $method) {
            $url.= ((false === strpos($url, '?'))?'?':'&').$params;
        }
    }
    $context = stream_context_create($opts);
    $cnt = 0;
    $file = false;
    //尝试$times次获取远程文件
    while($cnt < $times && ($file = file_get_contents($url, false, $context)) === false)$cnt++;

    return $file;
}


?>
