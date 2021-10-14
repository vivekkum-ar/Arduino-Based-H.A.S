<?PHP
    function sendMessage(){
        $content = array(
            "en" => 'We have detected a tresspasser' ,      // you can use some other message here
            );
        
        $fields = array(
            'app_id' => "your app_id here",
            'include_player_ids' => array("your player id here"), /*use multiple as array("player-id-1","player-id-2","player-id-3",.....)*/
            'data' => array("foo" => "bar"),
            "headings" => array("en" => "Intruder Alert !"),
            "large_icon" => "https://res.cloudinary.com/akp-assistant/image/upload/v1619695374/SHlogo.png",
            "priority" => 10,
            'contents' => $content
        );
        
        $fields = json_encode($fields);
        print("\nJSON sent:\n");
        print($fields);
        
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_URL, "https://onesignal.com/api/v1/notifications");
        curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-Type: application/json; charset=utf-8'));
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
        curl_setopt($ch, CURLOPT_HEADER, FALSE);
        curl_setopt($ch, CURLOPT_POST, TRUE);
        curl_setopt($ch, CURLOPT_POSTFIELDS, $fields);
        curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);

        $response = curl_exec($ch);
        curl_close($ch);
        
        return $response;
    }
    
    $response = sendMessage();
    $return["allresponses"] = $response;
    $return = json_encode( $return);
    
    print("\n\nJSON received:\n");
    print($return);
    print("\n");
?>