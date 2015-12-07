# 433MHz-decoder
Decoder for 433MHz using SDR


// Build and use
make
rm /tmp/gnu_radio_out
mkfifo /tmp/gnu_radio_out
./float_to_bit /tmp/gnu_radio_out /dev/stdout 5 | ./rx_decoder_433MHz /dev/stdin 0
//Start GNU-radio with 433MHz-Decoder.grc
//or
python decoder_433MHz.py


//Set the gain so that the max is just below 1
//open the file in audacity to check the sample rate
// File/raw data/
// 32bit float, set ample rate to 
//Change the target_samp_rate so that you can easily se the samp rate in audacity


//GIT lazy
git clone https://github.com/danielbrogren/433MHz-Decoder
//Create new branch
git branch <new branch>



//Changes to the new branch
git checkout <new branch>

//do changes
git status

//add new files
git add <filename>

//Saves the changes on the branch
git commit -a -m "feature name" 

//get the latest changes from repository
git pull 

//Push local info to repository
git push --mirror https://github.com/danielbrogren/433MHz-Decoder

//Do pullrequest in web-interface

