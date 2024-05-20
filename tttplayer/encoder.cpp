#include "encoder.h"
#include "events.h"
#include "types.h"
#include "other.h"
#include <cstdint>

/*EncodedMessage generateMessageRequest(char typeOfMessage) const {
	Messages current;
	switch (typeOfMessage) {	  //"download" current message 
	case 'F':  current.state = _state;		 current.msgType = '1';	break;
	case 'S':  current.settings = _settings; current.msgType = '2';	break;
	default:   current.event = _event;		 current.msgType = '3'; break;
	}

	EncodedMessage msg(current);  //encode message;
	return msg;
}
*/

void add_int(int a, char* place) {
	int* a0 = &a;
	char* place0 = reinterpret_cast<char*>(a0);
	rep(4) {
		*(place + i) = *(place0 + i);
	}
}

int get_int(const char* place) {
	const int* place0 = reinterpret_cast<const int*>(place);
	return *place0;
}

int count_marks(const Field& field, Mark mark_type) {
	int counter = 0;
	for (int i = field.get_current_boundary().min.x; i <= field.get_current_boundary().max.x; i++) {
		for (int j = field.get_current_boundary().min.y; j <= field.get_current_boundary().max.y; j++) {
			Point current_point;
			current_point.x = i;
			current_point.y = j;
			if (field.get_value(current_point) == mark_type) counter++;
		}
	}
	return counter;
}

/*
char* EncodedMessage::encodePlayerJoinedEvent(const Messages message) {
	
	char* msg = new char[5 + 1 + n];
	struct Data {
		char type;
		int32_t full_size;
		char mark;
	};
	Data* ptr = reinterpret_cast<Data*>(msg);
	ptr->type = '2';
	ptr->full_size = 1 + n;
	
	}
}
*/

//=====================================================================================
//=======================================ENCODER=======================================
//=====================================================================================


size_t MessageEncoder::determineSize(const Field& field) {
	size_t size = 1 + 4 + 4 + 4; //msg_type + msg_size + x_counter + o_counter +...
	size += (count_marks(field, Mark::Cross) + count_marks(field, Mark::Zero)) * 8; //...+ marks_location
	return size;
}

EncodingStatus MessageEncoder::encodeGameField(char* dst_buf, size_t buf_size, const Field& field) {
	dst_buf[0] = 'F';
	add_int(buf_size - 5, &dst_buf[1]);
	add_int(count_marks(field, Mark::Cross), &dst_buf[5]);
	add_int(count_marks(field, Mark::Zero), &dst_buf[9]);
	int current_index = 9 + 4; 
	rep(2) {
		Mark mark;
		if (i == 0) mark = Mark::Cross;
		else mark = Mark::Zero;
		for (auto it = field.get_iterator(); it->has_value(); it->step()) {
			if (it->get_value() != mark) continue;
			Point current = it->get_point();
			add_int(current.x, &dst_buf[current_index]);
			current_index += 4;
			add_int(current.y, &dst_buf[current_index]);
			current_index += 4;
		}
	}
	return OK;
}

size_t MessageEncoder::determineSize(const GameSettings& settings) {
	return 1 + 4 + 4*4 + 4 + 4 + 4; // msg_type, msg_size, boundaries, max_moves, win_length, move_time_limit
}

EncodingStatus MessageEncoder::encodeGameSetting(char* dst_buf, size_t buf_size, const GameSettings& state) {
	dst_buf[0] = 'S';
	add_int(28, &dst_buf[1]);
	add_int(state.field_size.min.x, &dst_buf[5]);
	add_int(state.field_size.min.y, &dst_buf[9]);
	add_int(state.field_size.max.x, &dst_buf[13]);
	add_int(state.field_size.max.y, &dst_buf[17]);
	add_int(state.max_moves, &dst_buf[21]);
	add_int(state.win_length, &dst_buf[25]);
	add_int(state.move_timelimit_ms, &dst_buf[29]);
	return OK;
}

size_t MessageEncoder::determineSize(const Event& event) {
	std::string type = event.get_type();
	if (type == "move") return 5 + 9; //Xpos(4), Ypos(4), mark(1)

	else if (type == "player_joined") {
		int n = get_data<PlayerJoinedEvent>((event)).name.length();
		return 5 + 1 + n; //mark(1), name(n)
	}

	else if (type == "game_started") return 5; // -

	else if (type == "win") return 5 + 1; //mark(1)

	else if (type == "draw") {
		int n = get_data<DrawEvent>((event)).reason.length();
		return 5 + n; // reason(n)
	}

	else if (type == "disqualification") {
		int n = get_data<DisqualificationEvent>((event)).reason.length();
		return 5 + 1 + n; // mark(1), reason(n)
	}

	else if (type == "message") {
		int n1 = get_data<MessageEvent>((event)).source.length();
		int n2 = get_data<MessageEvent>((event)).message.length();
		return 5 + 4 + 4 + n1 + n2; // n1, n2, source(n1), message(n2)
	}

	return 5; // default
}

EncodingStatus MessageEncoder::encodeGameEvent(char* dst_buf, size_t buf_size, const Event & event) {
	std::string type = event.get_type();
	if (type == "move") {
		dst_buf[0] = '1'; //'1' - msg type
		add_int(9, &dst_buf[1]);										//msg.len (9 = int+int+bool)						
		add_int(get_data<MoveEvent>((event)).point.x, &dst_buf[5 + 0]);	//x position
		add_int(get_data<MoveEvent>((event)).point.y, &dst_buf[5 + 4]);	//Y position
		if (get_data<MoveEvent>((event)).mark == Mark::Cross) dst_buf[5 + 5] = 1;	 //is cross
		else if (get_data<MoveEvent>((event)).mark == Mark::Zero) dst_buf[5 + 5] = 0;//is zero
		else return MARK_IS_EMPTY;										 //is empty
		return OK;
	}
	
	else if (type == "player_joined") {
		int n = get_data<PlayerJoinedEvent>((event)).name.length();
		dst_buf[0] = '2'; //'2' - msg type
		add_int(1 + n, &dst_buf[1]);
		if (get_data<PlayerJoinedEvent>((event)).mark == Mark::Cross) dst_buf[5 + 0] = 1;	 //1 == plays by cross
		else if (get_data<PlayerJoinedEvent>((event)).mark == Mark::Zero) dst_buf[5 + 0] = 0;//0 == plays by zero
		else return MARK_IS_EMPTY;
		rep(n) {
			dst_buf[5 + 1 + i] = get_data<PlayerJoinedEvent>((event)).name[i];				 //nickname
		}
		return OK;
	}

	else if (type == "game_started") {
		dst_buf[0] = '3'; //'3' - msg type
		add_int(0, &dst_buf[1]);
		return OK;
	}

	else if (type == "win") {
		dst_buf[0] = '4'; //'4' - msg type
		add_int(1, &dst_buf[1]);
		if (get_data<WinEvent>((event)).winner_mark == Mark::Cross) dst_buf[5 + 0] = 1;	   //0 == cross wins
		else if (get_data<WinEvent>((event)).winner_mark == Mark::Zero) dst_buf[5 + 0] = 0;//0 == zero wins
		else return MARK_IS_EMPTY;								
		return OK;
	}

	else if (type == "draw") {
		int n = get_data<DrawEvent>((event)).reason.length();
		dst_buf[0] = '5';
		add_int(n, &dst_buf[1]);
			rep(n) {
			dst_buf[5 + i] = get_data<DrawEvent>((event)).reason[i];
		}
		return OK;
	}

	else if (type == "disqualification") {
		int n = get_data<DisqualificationEvent>((event)).reason.length();
		char* dst_buf = new char[5 + 1 + n];
		dst_buf[0] = '6';
		add_int(n + 1, &dst_buf[1]);
		if (get_data<DisqualificationEvent>((event)).player_mark == Mark::Cross) dst_buf[5 + 0] = 1;
		else if (get_data<DisqualificationEvent>((event)).player_mark == Mark::Zero) dst_buf[5 + 0] = 0;
		else return MARK_IS_EMPTY;
		rep(n) {
			dst_buf[5 + 1 + i] = get_data<DisqualificationEvent>((event)).reason[i];
		}
		return OK;
	}

	else if (type == "message") {
		int n1 = get_data<MessageEvent>((event)).source.length();
		int n2 = get_data<MessageEvent>((event)).message.length();
		dst_buf[0] = '7';
		add_int(n1 + n2, &dst_buf[1]);
		add_int(n1, &dst_buf[5]);
		add_int(n2, &dst_buf[9]);
		rep(n1) {
			dst_buf[13 + i] = get_data<MessageEvent>((event)).source[i];
		}
		rep(n2) {
			dst_buf[13 + n1 + i] = get_data<MessageEvent>((event)).message[i];
		}
		return OK;
	}

	else return ERROR_TYPE;
}

//=====================================================================================
//=======================================DECODER=======================================
//=====================================================================================


MessageType MessageDecoder::determineMessageType(const char* msg, size_t msg_size) {
	if (msg != nullptr){
		char x = msg[0];
		if (x == 'F') return MessageType::FIELD;
		if (x == 'S') return MessageType::SETTINGS;
		if (x == '1' || x == '2' || x == '3' || x == '4' || x == '5' || x == '6' || x == '7') return MessageType::EVENT;
	}
	return MessageType::ERROR;
}

Event MessageDecoder::decodeEvent(const char* msg, size_t msg_size) {
	
	if (msg[0] == '1') { //move
		Point point = { .x = get_int(&msg[5]), .y = get_int(&msg[9]) };
		Mark mark;
		if (msg[10] == 1) mark = Mark::Cross;
		else if (msg[10] == 0) mark = Mark::Zero;
		else mark = Mark::None;
		return MoveEvent::make(point, mark);
	}
	else if (msg[0] == '2') { //player joined
		int n = get_int(&msg[1]) - 1;
		std::string name;
		Mark mark;
		if (msg[5] == 1) mark = Mark::Cross;
		else if (msg[5] == 0) mark = Mark::Zero;
		else mark = Mark::None;
		rep(n) {
			name[i] = msg[5 + 1 + i];
		}
		return PlayerJoinedEvent::make(name, mark);
	}
	else if (msg[0] == '3') { //game started
		return GameStartedEvent::make();
	}
	else if (msg[0] == '4') { //win
		Mark mark;
		if (msg[5] == 1) mark = Mark::Cross;
		else if (msg[5] == 0) mark = Mark::Zero;
		else mark = Mark::None;
		return WinEvent::make(mark);
	}
	else if (msg[0] == '5') { //draw
		std::string reason;
		int n = get_int(&msg[1]);
		rep(n) {
			reason[i] = msg[5 + 1 + i];
		}
		return DrawEvent::make(reason);
	}
	else if (msg[0] == '6') { //disqualification
		std::string reason;
		int n = get_int(&msg[1]) - 1;
		Mark mark;
		if (msg[5] == 1) mark = Mark::Cross;
		else if (msg[5] == 0) mark = Mark::Zero;
		else mark = Mark::None;
		rep(n) {
			reason[i] = msg[5 + 1 + i];
		}
		return DisqualificationEvent::make(reason, mark);
	}
	else if (msg[0] == '7') { //message
		std::string source;
		std::string message;
		int n1 = get_int(&msg[5]);
		int n2 = get_int(&msg[9]);

		rep(n1) {
			source[i] = msg[13 + i];
		}
		rep(n2) {
			message[i] = msg[13 + n1 + i];
		}
		return MessageEvent::make(source, message);
	}
	else {
		return Event ("error type", nullptr);
	}
}

void MessageDecoder::decodeField(const char* msg, size_t msg_size, Field& field) {
	int crosses_counter = get_int(&msg[5]);
	int zeros_counter = get_int(&msg[9]);

	int current_index = 9 + 4;
	rep(crosses_counter) {
		Point current = { .x = get_int(&msg[current_index]), .y = get_int(&msg[current_index + 4]) };
		field.set_value(current, Mark::Cross);
	}

	rep(zeros_counter) {
		Point current = { .x = get_int(&msg[current_index]), .y = get_int(&msg[current_index + 4]) };
		field.set_value(current, Mark::Zero);
	}
}

GameSettings MessageDecoder::decodeSettings(const char* msg, size_t msg_size) {
	GameSettings settings;
	settings.field_size.min.x = get_int(&msg[5]);
	settings.field_size.min.y = get_int(&msg[9]);
	settings.field_size.max.x = get_int(&msg[13]);
	settings.field_size.max.y = get_int(&msg[17]);
	settings.max_moves = get_int(&msg[21]);
	settings.win_length = get_int(&msg[25]);
	settings.move_timelimit_ms = get_int(&msg[29]);
	return settings;
}