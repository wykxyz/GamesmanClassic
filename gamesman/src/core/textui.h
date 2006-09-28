
#ifndef GMCORE_TEXTUI_H
#define GMCORE_TEXTUI_H

void		HitAnyKeyToContinue	();
char		GetMyChar		();
int		GetMyInt		();
void		GetMyHelper		(char *format, GENERIC_PTR target);
void		GetMy			(char *format, GENERIC_PTR target, int length);
void		GetMyStr		(STRING str, int len);
void		BadMenuChoice		();

void		Menus			();
void		HelpMenus		();

USERINPUT	HandleDefaultTextInput	(POSITION pos, MOVE* move, STRING name);
void		showStatus		(STATICMESSAGE msg);



#endif /* GMCORE_TEXTUI_H */
