#include "DisplaySystem.h"

#ifdef COMPILE_DISPLAY_SYSTEM_

TextField tfCmd;
TextField tfClockH;
TextField tfClockM;
TextField tfClockS;
ProgressBar proBar;

void DisplaySystem_init(void)
{
	TextField tfClockColons;//冒号
	Border bdwin;
	
	lcd18_setBorder(&bdwin);
	
	//===============画边框
	bdwin.x=0;
	bdwin.y=0;
	bdwin.width=6;
	bdwin.height=LCD18_H;
	bdwin.color=dosBorder;
	bdwin.colorCount=6;
	bdwin.position=BDLeft;//左
	lcd18_drawBorder();
	
	bdwin.width=LCD18_W;
	bdwin.height=6;
	bdwin.position=BDTop;//上
	lcd18_drawBorder();
	
	bdwin.y=LCD18_H-6;
	bdwin.position=BDBottom;//下
	lcd18_drawBorder();
	
	bdwin.x=LCD18_W-6;
	bdwin.y=0;
	bdwin.width=6;
	bdwin.height=LCD18_H;
	bdwin.position=BDRight;//右
	lcd18_drawBorder();
	
	bdwin.x=3;
	bdwin.y=22;
	bdwin.width=LCD18_W-5;
	bdwin.height=3;
	bdwin.color=&dosBorder[3];
	bdwin.colorCount=3;
	bdwin.position=BDTop;//上格条
	lcd18_drawBorder();
	
	bdwin.y=LCD18_H-16;
	bdwin.position=BDTop;//下格条
	lcd18_drawBorder();
	
	//时 TextField
	tfClockH.x=6;
	tfClockH.y=6;
	tfClockH.width=16;
	tfClockH.height=16;
	tfClockH.foreColor=RED;
	tfClockH.backColor=LIGHTBLUE;
	tfClockH.font=&Digital;
	
	lcd18_initTextField(&tfClockH);
	//分 TextField
	tfClockM.x=tfClockH.x+24;
	tfClockM.y=tfClockH.y;
	tfClockM.width=tfClockH.width;
	tfClockM.height=tfClockH.height;
	tfClockM.foreColor=RED;
	tfClockM.backColor=tfClockH.backColor+50;
	tfClockM.font=&Digital;
	
	lcd18_initTextField(&tfClockM);
	//秒 TextField
	tfClockS.x=tfClockM.x+24;
	tfClockS.y=tfClockH.y;
	tfClockS.width=tfClockH.width;
	tfClockS.height=tfClockH.height;
	tfClockS.foreColor=RED;
	tfClockS.backColor=tfClockM.backColor+50;
	tfClockS.font=&Digital;
	
	lcd18_initTextField(&tfClockS);
	
	//冒号 TextField
	tfClockColons.x=tfClockH.x;
	tfClockColons.y=tfClockH.y;
	tfClockColons.width=64;
	tfClockColons.height=tfClockH.height;
	tfClockColons.foreColor=BLACK;
	tfClockColons.backColor=tfClockH.backColor-50;
	tfClockColons.font=&Digital;
	
	lcd18_initTextField(&tfClockS);
	
	//主输出框TextField
	tfCmd.backColor=BLACK;
	tfCmd.x=6;
	tfCmd.y=25;
	tfCmd.width=LCD18_W-9;
	tfCmd.height=LCD18_H-41;
	tfCmd.foreColor=WHITE;
	tfCmd.font=&Fixedsys;
	
	lcd18_initTextField(&tfCmd);
	
	//下方进度条 ProgressBar
	proBar.x=3;
	proBar.y=LCD18_H-15;
	proBar.width=123;
	proBar.height=13;
	proBar.backColor=GRAY;
	proBar.foreColor=BLUE;
	
	lcd18_initProgressBar(&proBar);
	lcd18_setProgressBar(&proBar);
	lcd18_setPBVal(44);
	
	lcd18_setTextField(&tfClockColons);
	lcd18_printStr("  :  :");
	lcd18_setTextField(&tfClockH);
	lcd18_printStr("00");
	lcd18_setTextField(&tfClockM);
	lcd18_printStr("00");
	lcd18_setTextField(&tfClockS);
	lcd18_printStr("00");
	
	lcd18_setTextField(&tfCmd);
	lcd18_clrTextField();
}

#endif
