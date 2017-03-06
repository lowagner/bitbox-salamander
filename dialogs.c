
// dialogs for town
int town_dialog() {
	ans = window_dialog (PNJ1, 
		"Hi boy, what are you doing alone ? I thought your father never let you go out alone, that's nice !", 
		"I'm just minding my business, and it's none of yours\n" 
		"My father sent me to look for help, he's not well !\n"
		"I'm currently waiting for him, but maybe you've seen him ?"
	)

	switch (ans) {
		case 0 : 
			window_dialog (PNJ1, "Young boy, I'm your friend father, and I don't like the way you talk to me.\n"
				"And the fact that you're alone at this late hour is my business indeed.\n"
				"Come here, I'll bring you home.",0);
			return 1;
			break;
		case 1 : 
			window_dialog (PNJ1, 
				"Oh, your father is in bad shape ? Well, it's nice that you found me,"
				" I'll never let him down alone. We're going to your house at once !\n", 0);
			window_dialog (ME, "Oh, well ...",0);
			return 0;
			break;
		case 2 : 
			window_dialog (PNJ1," Ah, no I haven't seen him just here. In fact I was ...\n"
				"returning from the Happy Boar tavern and returning home.\n"
				"Good luck in whatever you're doing tonight !\n",0);
			window_dialog (ME, "It seemed to work ...",0);
			return 2;
	}
}

/*
=====================================================
intro:You don't look like you're from around here 
		  I've lived here all my life ! -> bluff
		  I came here from Newton -> newton

newton: Newton, eh ? I heard there's trouble brewing down there
		  Did I say Newton ? I'm actually from Springville -> bluff
		  I haven't heard about any trouble -> OK

bluff : Oh really ? Then you must know Mr Bowler.
		  Who ? -> OK
		  Mr Bowler is a good friend of mine -> liar

liar : You liar ! There ain't no Mr Bowler, I made him up !
		  Ooops ... -> exit_2
		  
OK : Don't Worry about it. Say, do you have something to eat ? I'm starving !
		  OK -> exit_1
*/
int dlg_ok();
int dlg_bluff();

int forest_dialog()
{
	if (!globals.seenguy) {
		int ans = window_dialog(PNJ1, 
			"You don't look like you're from around here",
			"I've lived here all my life ! \n I came here from Newton"
			);

		if (ans==0) {
			dlg_bluff;
		} else {
			ans=window_dialog(PNJ1, "Newton, eh ? I heard there's trouble brewing down there",
			  "Did I say Newton ? I'm actually from Springville \n"
			  "I haven't heard about any trouble"
			  );

			return ans==0 ? dlg_bluff() : dlg_ok();
		}
	} else {
		window_dialog(PNJ1, "Hi again ! You can go on.",0);
	}
}

int dlg_ok()
{
	window_dialog(PNJ1, "Don't Worry about it. Say, do you have\nsomething to eat ? I'm starving !",0);
	globals.seenguy = 1;
	return 0;
}

int dlg_bluff() {
	ans = window_dialog(PNJ1, "Oh really ? Then you must know Mr Bowler.", 
		 "Who ? \n Mr Bowler is a good friend of mine !");

	if (ans==0) {
		return dlg_ok();
	} else {
		window_dialog(PNJ1, "You liar ! There ain't no Mr Bowler, I made him up !",0);
		return 1;
	}

}
