<?php

$host_name = '';
$database = '';
$user_name = '';
$password = '';

$secret_token = 'sdhsd8sdhd';

$dbh = null;
$res = null;

//Connect to the database
try {
  $dbh = new PDO("mysql:host=$host_name; dbname=$database;", $user_name, $password);
} catch (PDOException $e) {
  echo "Error!: " . $e->getMessage() . "<br/>";
  die();
}

//Check for posts, if so, insert in database
if (!empty($_GET["temp"])) {
    //Retrieve values
    $temp = $_GET["temp"];
    $hum = $_GET["hum"];
    $room_id = $_GET["room_id"];
    $hash = $_GET["hash"];
    $code = $_GET["code"];

    //Sanitize the variables that will interact with the database.
    $temp = preg_replace("/[^A-Za-z0-9.]/", "", $temp);
    $hum = preg_replace("/[^A-Za-z0-9.]/", "", $hum);
    $hash = preg_replace("/[^A-Za-z0-9]/", "", $hash);
    $room_id = preg_replace("/[^A-Za-z0-9]/", "", $room_id);

    //Check if token is correct
    if (md5($code.$secret_token)==$hash) {
        //Check if code has been reused
        //Create SQL    
        $sql="SELECT * FROM codes WHERE code='".$hash."';";
        //Query database
        $res = $dbh->query($sql);
        //If rowcount is not greater than 0 then this is a new code and we can proceed.                
        if(!$res->fetchColumn() > 0){
            //Insert code in database
                $sql="INSERT INTO codes (code) VALUES ('".$hash."');";
                $res = $dbh->query($sql);
            //Insert temp and hum in database
                $sql="INSERT INTO temperature (room_id, temperature) VALUES ('".$room_id."', '".$temp."');";
                $res = $dbh->query($sql);
                $sql="INSERT INTO humidity (room_id, humidity) VALUES ('".$room_id."', '".$hum."');";
                $res = $dbh->query($sql);
            //Feedback
                echo "Values saved";
        } else {
            echo "Code reused";
        }
    } else {
        echo "Hash failed";
    }
} else {
	//This is just a user visiting the page, so get the results from the database and display them
	
	include("graph_header.inc");

	$sql = "SELECT a.temperature, b.humidity, a.Timestamp FROM temperature a, humidity b where a.temp_measurement_id=b.humidity_measurement_id";
	$firstrow = true;
	
	foreach($dbh->query($sql) as $row){
		if (!$firstrow) {
			echo ",";
		}
		$firstrow=false;
		
		//Convert Commie units to Freedom units
		$fahrenheit = (9/5)*$row['temperature']+32;
		
		echo "['{$row['Timestamp']}',".$fahrenheit.",{$row['humidity']}]";
	}
	
	include("graph_footer.inc");	
}
?>


