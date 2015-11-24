# 433MHz-decoder
Decoder for 433MHz using SDR


// Build and use
gcc -o float_to_bit float_to_bit.c
rm /tmp/gnu_radio_out
mkfifo /tmp/gnu_radio_out
./float_to_bit -v /tmp/gnu_radio_out /dev/stdout
//Start GNU-radio with 433MHz-Decoder.grc
//or
python top_block.py




//GIT lazy
git clone https://github.com/danielbrogren/433MHz-Decoder
//Create new branch
git branch <new branch>

//Changes to the new branch
git checkout <new branch>

//do changes

//Saves the changes on the branch
git commit -a -m "feature name" 

//get the latest changes from repository
git pull 

//Push local info to repository
git push --mirror https://github.com/ghuser/repo.git

//Do pullrequest in web-interface

