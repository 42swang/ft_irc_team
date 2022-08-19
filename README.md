### 선택한 클라이언트 : irssi  
  [irssi (깃허브)](https://github.com/irssi/irssi)  
  [irssi (홈페이지)](https://irssi.org/)  
  
**irssi 주요 명령어**  
`/connect` \<ip> \<port> \<password> : 서버 접속 명령어  
-> 모든 인자가 한번에 입력되어야 한다. (nc 명령어는 쪼개서 보낼 수 있음)  
  
`/window` left, `/window` right : 화면 이동(채팅방 이동)  
-> 메인화면, 채팅화면 & DM 화면으로 이루어져 있음, 왼쪽 오른쪽으로 화면을 넘나들 수 있다.  
  
`/msg` \<target> : PRIVMSG 명령어  
-> \<target>이 #로 시작하면 채널, 아니면 유저로 취급하여 irssi가 알아서 프로토콜에 맞게 서버에 파싱해준다.  
-> 보고 있는 화면의 텍스트 입력창에선 /msg 명령어를 입력하지 않아도 알아서 서버에게 보내줌. 다른 채팅방이나 사람에게 보낼땐 명령어 필요함.

`/nick` \<변경할닉네임>  
-> 닉네임 변경 (중복체크 필수) 

`/join` \<채널이름,>  
-> 채널 접속. `,`로 구분되어 여러채널에 동시에 접속 가능함  

`/part` \<채널이름,> <메세지>  
-> 채널 퇴장. 역시 `,`로 구분된 인자를 받아 여러 채널에서 동시 퇴장이 가능하다. 퇴장시 채널에 `[ ]` 기호 안에 메세지가 담겨 출력됨

`/kick` \<유저이름> \<메세지>  
-> 유저 추방. 메세지를 추가로 보낼 수 있음. 퇴장시 채널에 `[ ]` 기호 안에 메세지가 담겨 출력됨

`/quit` \<이유>  
-> 클라이언트 종료. 메세지를 추가로 보낼 수 있음. 퇴장시 채널에 `[ ]` 기호 안에 메세지가 담겨 출력됨

`/notice` \<유저이름> \<메세지>  
-> 개인 메세지, DM창이 따로 열리지 않고 수신자와 발신자 모두 메인화면에 택스트가 나타남

**irssi 포인트**
- irssi가 서버로 PING 프로토콜을 보냄, PONG 응답을 보내지 않으면 `Lag:시간`이 점점 증가하고 결국 연결이 끊김  
- welcome 과정이 `/connect`명령어로 한번에 처리가 가능하나 nc 명령어로 테스트하기 위해 구체적인 구현이 필요함  
  [welcome 프로토콜 차이점 정리](https://github.com/42swang/ft_irc_team/wiki/Welcome-%ED%94%84%EB%A1%9C%ED%86%A0%EC%BD%9C-%EA%B3%BC%EC%A0%95)

### 참고한 레퍼런스 페이지  
  [rfc1459](https://datatracker.ietf.org/doc/html/rfc1459)  
  [rfc2812](https://datatracker.ietf.org/doc/html/rfc2812)
### CAP LS 관련 참고한 사이트  
  [ircv3](https://ircv3.net/specs/extensions/capability-negotiation.html)

---
[ft_irc_team 위키 바로가기](https://github.com/42swang/ft_irc_team/wiki)
