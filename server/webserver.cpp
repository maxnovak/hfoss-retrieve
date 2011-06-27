/*
   WebServer.cpp

   Copyright (C) 2003-2004 Ren� Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   Ren� Nyffenegger rene.nyffenegger@adp-gmbh.ch

   Thanks to Tom Lynn who pointed out an error in this source code.
*/

#include <ctime>
#include <process.h>
#include <iostream>
#include <string>
#include <map>
#include <sstream>
//<process.h>


#include "webserver.h"
#include "socket.h"
#include "UrlHelper.h"
#include "base64.h"



#include "highgui.h"
#include "stdafx.h"
webserver::request_func webserver::request_func_=0;

unsigned webserver::Request(void* ptr_s) {
  Socket s = *(reinterpret_cast<Socket*>(ptr_s));
  std::string instr;
  std::cout << "RR" << std::endl;


  char inbuf[ 2000 ];
  s.ReceiveBytes(inbuf, 4);
  std::cout << (int)inbuf[0] << std::endl;
  std::cout << (int)inbuf[1] << std::endl;
  std::cout << (int)inbuf[2] << std::endl;
  std::cout << (int)inbuf[3] << std::endl;
  std::cout << (unsigned int)(*((unsigned int*)inbuf)) << std::endl;

  unsigned int pic_len = (unsigned int)(*((unsigned int*)inbuf));
  //char * pic_buf = (char *) malloc( pic_len );
  
  //char pic_buf[10];
  //s.ReceiveBytes(pic_buf, pic_len);

  char *pic_buf;
  
  pic_buf= new char[pic_len];


 unsigned int buf_size=8;
 unsigned int index=0;


 
 while (index<pic_len){
	
    unsigned int length;
	if (pic_len-index>buf_size){length=buf_size;}
	else{length=pic_len-index;}

	char *pic_buf0;
    pic_buf0= new char[length];
	s.ReceiveBytes(pic_buf0, length);

	for (unsigned int k=0;k<length;k++){
		pic_buf[index]=pic_buf0[k];
		index++;
	}	


	delete[] pic_buf0;
  }
  //s.ReceiveBytes(pic_buf, pic_len);



  //std::cout << "LENGTH " << pic_len << std::endl;

  //for(int i=0; i < pic_len; i++ )
     //std::cout << (int)(pic_buf[i]) << std::endl;
  

  FILE* fp = NULL;
  fp = fopen( "image2.jpg", "wb" );
  fwrite (pic_buf,1, pic_len, fp);
  fclose( fp );







   IplImage* img = cvLoadImage( "image2.jpg" );
   cvNamedWindow( "Example1", CV_WINDOW_AUTOSIZE );
   cvShowImage( "Example1", img );
   cvWaitKey(0);
   cvReleaseImage( &img );








   

   cvDestroyWindow( "Example1" );


  delete[] pic_buf;
  s.Close();
  return 0;





  std::string line = s.ReceiveLine();
  if (line.empty()) {
    return 1;
  }

  http_request req;

  if (line.find("GET") == 0) {
    req.method_="GET";
  }
  else if (line.find("POST") == 0) {
    req.method_="POST";
  }

  std::string path;
  std::map<std::string, std::string> params;

  size_t posStartPath = line.find_first_not_of(" ",3);

  SplitGetReq(line.substr(posStartPath), path, params);

  req.status_ = "202 OK";
  req.s_      = &s;
  req.path_   = path;
  req.params_ = params;

  static const std::string authorization   = "Authorization: Basic ";
  static const std::string accept          = "Accept: "             ;
  static const std::string accept_language = "Accept-Language: "    ;
  static const std::string accept_encoding = "Accept-Encoding: "    ;
  static const std::string user_agent      = "User-Agent: "         ;

  while(1) {
    line=s.ReceiveLine();

    if (line.empty()) break;

    unsigned int pos_cr_lf = line.find_first_of("\x0a\x0d");
    if (pos_cr_lf == 0) break;

    line = line.substr(0,pos_cr_lf);

    if (line.substr(0, authorization.size()) == authorization) {
      req.authentication_given_ = true;
      std::string encoded = line.substr(authorization.size());
      std::string decoded = base64_decode(encoded);

      unsigned int pos_colon = decoded.find(":");

      req.username_ = decoded.substr(0, pos_colon);
      req.password_ = decoded.substr(pos_colon+1 );
    }
    else if (line.substr(0, accept.size()) == accept) {
      req.accept_ = line.substr(accept.size());
    }
    else if (line.substr(0, accept_language.size()) == accept_language) {
      req.accept_language_ = line.substr(accept_language.size());
    }
    else if (line.substr(0, accept_encoding.size()) == accept_encoding) {
      req.accept_encoding_ = line.substr(accept_encoding.size());
    }
    else if (line.substr(0, user_agent.size()) == user_agent) {
      req.user_agent_ = line.substr(user_agent.size());
    }
  }

  request_func_(&req);

  std::stringstream str_str;
  str_str << req.answer_.size();

  time_t ltime;
  time(&ltime);
  tm* gmt= gmtime(&ltime);

  static std::string const serverName = "RenesWebserver (Windows)";

  char* asctime_remove_nl = asctime(gmt);
  asctime_remove_nl[24] = 0;

  s.SendBytes("HTTP/1.1 ");

  if (! req.auth_realm_.empty() ) {
    s.SendLine("401 Unauthorized");
    s.SendBytes("WWW-Authenticate: Basic Realm=\"");
    s.SendBytes(req.auth_realm_);
    s.SendLine("\"");
  }
  else {
    s.SendLine(req.status_);
  }
  s.SendLine(std::string("Date: ") + asctime_remove_nl + " GMT");
  s.SendLine(std::string("Server: ") +serverName);
  s.SendLine("Connection: close");
  s.SendLine("Content-Type: text/html; charset=ISO-8859-1");
  s.SendLine("Content-Length: " + str_str.str());
  s.SendLine("");
  s.SendLine(req.answer_);

  s.Close();
  

  return 0;
}

webserver::webserver(unsigned int port_to_listen, request_func r) {
  SocketServer in(port_to_listen,5);

  request_func_ = r;

  while (1) {
    Socket* ptr_s=in.Accept();

    unsigned ret;
    _beginthreadex(0,0,Request,(void*) ptr_s,0,&ret);
  }
}