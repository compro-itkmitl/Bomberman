using namespac std; // for typdef sf::xx to xx

Time xxx;  //type for construct time values ex.seconds milliseconds micro..
	xxx = seconds(float);			xxx.asSeconds(); //convert elapsed time to seconds
	xxx = milliseconds(float);		xxx.asMilliseconds(); // convert elapsed time to millisconds

Clock xxx; //start clock for count time in program
	xxx.getElapsedTime(); // retrieve the time elapsed since the clock started
	xxx.restart(); //restart also return the elapsed time

Thread xxx(&"function", parameter);  //create thread
	xxx.launch(); // use when we need a thread working on
	xxx.wait(); // block excueition untill main thread is finished
	//use mutex for protect memory when use many threads

sleep(milliseconds()); // sleep

//  all threads in programming share the same memory



FileStream xxx;
	xxx.open("file name.type");
Texture yyy;
	yyy.loadFromStream(xxx);