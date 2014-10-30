<?
// Set your Application Token here
define('TOKEN', '123456789012345678901234567890');
// Set your User Key here
define('USER', 'abcdefghijabcdefghijabcdefghij');

// All responses should be JSON
header('Content-type: application/json');

// Security check!
// Set "Referer" = "your_web_host.com" in request headers
if(!isset($_SERVER['HTTP_REFERER']) || substr_count($_SERVER['HTTP_REFERER'], $_SERVER['SERVER_NAME'])==0)
  die(json_encode(array(
    'error' => 'Invalid request'
  )));

// Build the URL.  Since it's possible to accidentally have an
// extra / or two in $_SERVER['QUERY_STRING], replace "//" with "/"
// using str_replace().  This also appends the access token to the URL.
//
//$url = 'https://api.pushover.net/1/messages.json'.'?token='.'123456789012345678901234567890'.'&user='.'abcdefghijabcdefghijabcdefghij'.'&title='.'php'.'&message='.'hello';
$url = 'https://api.pushover.net/1/messages.json'.'?token='.TOKEN.'&user='.USER.'&timestamp='.time().str_replace('//', '/', $_SERVER['QUERY_STRING']);

// HTTP POST requires the use of cURL
if (strtoupper($_SERVER['REQUEST_METHOD'])=='POST') {
  $c = curl_init();

  curl_setopt_array($c, array(
    // Set the URL to access
    CURLOPT_URL => $url,
    // Tell cURL it's an HTTP POST request
    CURLOPT_POST => TRUE,
    // Include the POST data
    // $HTTP_RAW_POST_DATA may work on some servers, but it's deprecated in favor of php://input
    CURLOPT_POSTFIELDS => file_get_contents('php://input'),
    // Return the output to a variable instead of automagically echoing it (probably a little redundant)
    CURLOPT_RETURNTRANSFER => TRUE
  ));

  // Make the cURL call and echo the response
  echo curl_exec($c);

  // Close the cURL resource
  curl_close($c);
}
?>