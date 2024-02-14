#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 

class Error {
protected:
    std::string error_message = "";
    int error_code = 0;
public:
    Error(int code, std::string message) : error_code{code}, error_message{ message } {
        std::cerr << error_message << "; code = " << code << "\n";
        exit(code);
    }
};

class FileError : public Error {
public:
    FileError(std::string message) : Error(1, message) {};
};

class FormatError : public Error {
public:
    FormatError(std::string message) : Error(2, message) {};
};

class ParrametrsError : public Error {
public:
    ParrametrsError(std::string message) : Error(3, message) {};
};

class ConfigFileError : public Error {
public:
    ConfigFileError(std::string message) : Error(4, message) {};
};

class CommandLineError: public Error {
public:
    CommandLineError(std::string message) : Error(5, message) {};
};

class WAV{
private:
    struct WAVHEADER{
        char chunkId[4];
        unsigned long chunkSize;
        char format[4];
        char subchunk1Id[4];
        unsigned long subchunk1Size;
        unsigned short audioFormat;
        unsigned short numChannels;
        unsigned long sampleRate;
        unsigned long byteRate;
        unsigned short blockAlign;
        unsigned short bitsPerSample;
        char subchunk2Id[4];
        unsigned long subchunk2Size;
    };
public:
    char* data;
    WAVHEADER header;
    WAV() = default;
    WAV(std::string file_name){
        read_wav(file_name);
    }
    ~WAV(){
        delete[] data;
    }
    WAV* read_wav(std::string file_name){
        std::ifstream file(file_name, std::ios::binary);
        if (file.fail()){
            FileError error("Error when open input WAV file");
        }
        file.read((char*)&this->header, sizeof(WAVHEADER) - 8);
        if (header.audioFormat != 1)
            FormatError error("Audio format isn't PMC (write \"sound_processor -h\" for help)");
        if (header.numChannels != 1)
            FormatError error("Audio isn't mono (write \"sound_processor -h\" for help)");
        if (header.bitsPerSample != 16)
            FormatError error("Audio isn't 16 bit (write \"sound_processor -h\" for help)");
        if (header.sampleRate != 44100)
            FormatError error("Audio can be only 44100 sampling rate (write \"sound_processor -h\" for help)");
        int subchunkInfoSize = 0;
        file.read((char*)&this->header.subchunk2Id, 4);
        if (header.subchunk2Id[0] == 'L' && header.subchunk2Id[1] == 'I' && header.subchunk2Id[2] == 'S' && header.subchunk2Id[3] == 'T'){
            file.read((char*)&subchunkInfoSize, 4);
            char* garbage = new char[subchunkInfoSize];
            file.read(garbage, subchunkInfoSize);
            file.read((char*)&this->header.subchunk2Id, 4);
        }
        file.read((char*)&this->header.subchunk2Size, 4);
        this->data = new char[this->header.subchunk2Size];
        file.read(data, this->header.subchunk2Size);
        return this;
    }
    void write_wav(std::string file_path){
        std::ofstream file(file_path, std::ios::binary);
        if (file.fail()){
            FileError error("Error when open/create output WAV file");
        }
        file.write((char*)&header, sizeof(WAVHEADER));
        file.write(data, header.subchunk2Size);
    }
    WAV* get_copy() {
        WAV* new_wav = new WAV;
        memcpy(&new_wav->header, &this->header, sizeof(header));
        new_wav->data = new char[this->header.subchunk2Size];
        memcpy(new_wav->data, this->data, this->header.subchunk2Size);
        return new_wav;
    }
};

class Convertor{
protected:
    int start = -1;
    int end = -1;
    WAV* wav_file = NULL;
public:
    Convertor() = default;
    Convertor(int start_time, WAV* wav_file_to_convert) : start{ start_time }, wav_file { wav_file_to_convert->get_copy()} {};
    Convertor(int start_time, int finish_time, WAV* wav_file_to_convert) : start{ start_time }, end{ finish_time }, wav_file { wav_file_to_convert } {};
    virtual void convert() {};
    WAV* get_wav() { return wav_file; }
    ~Convertor() = default;
};

class MuteWAV : public Convertor{
public:
    MuteWAV() : Convertor(){};
    MuteWAV(int start_time, int finish_time, WAV* wav_file_to_convert) : Convertor(start_time, finish_time, wav_file_to_convert){};
    void set_parametrs(int start_time, int finish_time, WAV* wav_file_to_convert){
        start = start_time;
        end = finish_time;
        wav_file = wav_file_to_convert->get_copy();
    }
    void convert(){
        if (end == -1 || start == -1 || wav_file == NULL){
            ParrametrsError error("Parrametrs for mute doesn't set");
        }
        int from_bytes = start * wav_file->header.byteRate;
        if (wav_file->header.subchunk2Size < from_bytes) {
            ParrametrsError error("Mute start time is greater than file duration");
        }
        int bytes_count = (end - start) * wav_file->header.byteRate;
        memset(wav_file->data + from_bytes, 0, bytes_count);
    }
    WAV* get_wav() { return Convertor::get_wav(); }
};

class MixWAV : public Convertor {
protected:
    WAV* wav_sub = NULL;
    short overflow_check(int input) {
        return (input > INT16_MAX ? INT16_MAX : (input < INT16_MIN ? INT16_MIN : input));
    }
public:
    MixWAV() : Convertor(){};
    MixWAV(int start_time, WAV* wav_file_to_convert, WAV* wav_file_sub) : Convertor(start_time, wav_file_to_convert), wav_sub{wav_file_sub->get_copy()} {};
    void set_parametrs(int start_time, WAV* wav_file_to_convert, WAV* wav_file_sub) {
        start = start_time;
        wav_file = wav_file_to_convert->get_copy();
        wav_sub = wav_file_sub->get_copy();
    }
    void convert() {
        if (start == -1 || wav_file == NULL) {
            ParrametrsError error("Parrametrs for mix doesn't set");
        }
        int from_bytes = start * wav_file->header.byteRate;
        if (wav_file->header.subchunk2Size < from_bytes) {
            ParrametrsError error("Mix start time is greater than file duration");
        }
        for (size_t i = from_bytes, j = 0; i < wav_file->header.subchunk2Size && j < wav_sub->header.subchunk2Size; i+= 2, j+= 2)
            *((short*)&(wav_file->data[i])) = overflow_check((int)*((short*)&(wav_file->data[i])) + (int)*((short*)&(wav_sub->data[j])));
    }
    WAV* get_wav() { return Convertor::get_wav(); }
};

class CropWAV : public Convertor {
public:
    CropWAV() : Convertor(){};
    CropWAV(int start_time, int finish_time, WAV* wav_file_to_convert) : Convertor(start_time, finish_time, wav_file_to_convert){};
    void set_parametrs(int start_time, int finish_time, WAV* wav_file_to_convert) {
        start = start_time;
        end = finish_time;
        wav_file = wav_file_to_convert->get_copy();
    }
    void convert() {
        if (end == -1 || start == -1 || wav_file == NULL) {
            ParrametrsError error("Parrametrs for crop doesn't set");
        }
        int from_bytes = start * wav_file->header.byteRate;
        if (wav_file->header.subchunk2Size < from_bytes) {
            ParrametrsError error("Crop start time is greater than file duration");
        }
        int to_bytes = end * wav_file->header.byteRate;
        if (wav_file->header.subchunk2Size < to_bytes) { 
            to_bytes = wav_file->header.subchunk2Size; 
        }
        int bytes_count = wav_file->header.subchunk2Size - to_bytes;
        memmove(&wav_file->data[from_bytes], &wav_file->data[to_bytes], bytes_count);
        int new_bytes_size = wav_file->header.subchunk2Size - (to_bytes - from_bytes);
        wav_file->data = (char*)realloc(wav_file->data, new_bytes_size);
        wav_file->header.subchunk2Size = new_bytes_size;
        wav_file->header.chunkSize -= (to_bytes - from_bytes);
    }
    WAV* get_wav() { return Convertor::get_wav(); }
};

class ConfigFilePlayer {
private:
    std::ifstream config_file;
    std::string out_file;
    const int wav_files_count = 0;
    char** files_paths = nullptr;
public:
    ConfigFilePlayer() = default;
    ConfigFilePlayer(std::string config_file_path, std::string out_file_path, int wavs_count, char** all_files, bool start_now = false) : wav_files_count{ wavs_count }, files_paths{all_files} {
        config_file.open(config_file_path, std::ios::binary);
        if (config_file.fail()) {
            FileError error("Error when open config file");
        }
        out_file = out_file_path;
        if (start_now)
            play_config_file();
    }
    void set_config_file(std::string file_path, std::string out_file_path) {
        config_file.open(file_path);
        out_file = out_file_path;
    }
    Convertor* create_converter(std::string type, int start, WAV* wav_file, int end = 0, WAV* sub_wav_file = nullptr) {
        if (type == "mute")
            return new MuteWAV(start, end, wav_file);
        if (type == "mix")
            return new MixWAV(start, wav_file, sub_wav_file);
        if (type == "crop")
            return new CropWAV(start, end, wav_file);
        return nullptr;
    }
    WAV* create_WAV(std::string file_path) {
        return new WAV(file_path);
    }
    void play_config_file() {
        if (wav_files_count == 0 || files_paths == nullptr){
            ParrametrsError error("Config parrametrs doesn't set");
        }
        WAV** WAVs = new WAV*[wav_files_count];
        for (int i = 0; i < wav_files_count; ++i){
            WAVs[i] = create_WAV(files_paths[i]);
        }
        WAV* result_wav = WAVs[0]->get_copy();
        std::string type = "";
        Convertor* convertor;
        for (std::string line; std::getline(config_file, line);)
        {
            if (line[0] == '#')
                continue;
            std::stringstream line_stream(line);
            line_stream >> type;    
            if (type == "mute" || type == "crop") {
                int start = 0, finish = 0;
                line_stream >> start >> finish;
                convertor = create_converter(type, start, result_wav, finish);
                convertor->convert();
            }
            else if (type == "mix") {
                int start = 0;
                std::string number_str = "";
                line_stream >> number_str >> start;
                int number = number_str[1] - '0' - 1;
                convertor = create_converter(type, start, result_wav, 0, WAVs[number]);
                convertor->convert();
            }
            else {
                std::cout << "Unknown comand in config file (proggram support only \"Mute\", \"Mix\", \"Cpor\"), program will try to perform the remaining conversions";
                continue;
            }
            result_wav = convertor->get_wav();
        }
        result_wav->write_wav(out_file);
    }
    ~ConfigFilePlayer() {
        if (config_file.is_open())
            config_file.close();
    }
};

int main(int argc, char **argv){
    if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'h') {
        std::cout << "This program is designed to convert WAV files\n!!!WAV files are supported only with the following characteristics:\n    ~PCM audio format(no encoding)\n    ~Mono(1 channel)\n    ~16 bit\n    ~with a frequency of 44100 Hz\n\nTransformations are specified via the config file. Comments are supported(lines starting with #)\nThe program supports such transformations as:\n    -Mute, mutes the sound at the selected interval(enter the config file \"mute start finish\")\n    -Mix, connects two audios(enter the config file \"mix $number_of_audio start_time\")\n    -Crop, cuts out a specified period(enter \"crop start finish\" in the config file)\n\Commands in the config file are executed sequentially, one after another\nTo start the program, enter:\n    Sound_Processor.exe -c config_file_name.txt name_of_1st_wav.wav names_of_others_wavs_files...\n";
        exit(0);
    }
    if (argc < 5) {
        CommandLineError error("Not enough parrametrs to start programm (write \"sound_processor -h\" for help)");
    }
    if (argv[1][0] == '-' && argv[1][1] == 'c') {
        ConfigFilePlayer config_player(argv[2], argv[3], argc - 4, &argv[4], true);
        exit(0);
    }
    CommandLineError error("Wrong command line parrametrs (maybe you forgot \"-c\"(for start programm) or \"-h\"(for help))");
}