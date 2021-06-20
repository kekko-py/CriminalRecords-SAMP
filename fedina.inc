//###############################################################################################################################
//
//                                                       Sistema Fedina Mysql
//                                                   
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//                                                 [|]_______________________________ 
//                                                |                                |]  
//                                               |  Developed by kekko.py [100%]  | 
//                                              |        ________________________|
//                                             |        |_]   |   
//                                            |        |_____/
//                                           |        |
//                                          |________| 
//
//                                                   |---- COFFEE TO CODE ----|  
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                             CREDITS: Francesco De Rosa [@kekko.py] 
//                              22/04/2020   Prima scrittura del codice [Francesco De Rosa (kekko.py)]
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define FEDINA_DIALOG1 3443
#define FEDINA_DIALOG2 3444
#define FEDINA_DIALOG3 3445

new ListReati[MAX_PLAYERS][50];
new SkipDialog[MAX_PLAYERS];
new IdSusAnalized[MAX_PLAYERS];

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stock RiduciStr(Stringa[200])
{
    if(strlen(Stringa)>40)
    {
        new OldStringa[200];
        format(OldStringa, sizeof(OldStringa), Stringa);
        format(Stringa, sizeof(Stringa), "");
        for(new i=0; i<35; i++) format(Stringa, sizeof(Stringa), "%s%c", Stringa, OldStringa[i]);
        format(Stringa, sizeof(Stringa), "%s...", Stringa);
    } 
    
    return Stringa;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
stock FormattaStr(Stringa[200])
{
    if(strlen(Stringa)>55)
    {
        new OldStringa[200];
        format(OldStringa, sizeof(OldStringa), Stringa);
        format(Stringa, sizeof(Stringa), "");
        new z,i;
        for(i=0; i<strlen(OldStringa); i++)
        
            if(i%53==0)
                if(OldStringa[i]==' ') format(Stringa, sizeof(Stringa), "%s\n", Stringa);
                else
                {
                    for(z=i; OldStringa[z]!=' ' && z<strlen(OldStringa); z++)  format(Stringa, sizeof(Stringa), "%s%c", Stringa, OldStringa[z]);
                    if(OldStringa[z]==' ') format(Stringa, sizeof(Stringa), "%s\n", Stringa);
                    i=z;
                }
            else format(Stringa, sizeof(Stringa), "%s%c", Stringa, OldStringa[i]);
    } 
    return Stringa;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stock GetReatoData(listitem, playerid)
{
    new QueryFedina[150];
    format(QueryFedina, sizeof(QueryFedina), "SELECT * FROM reati WHERE id='%i'",ListReati[playerid][listitem]);
    mysql_query(QueryFedina);
    mysql_store_result();

    if(mysql_num_rows() == 0) return ShowPlayerDialog(playerid,FEDINA_DIALOG1 ,DIALOG_STYLE_MSGBOX,"{086FA1}SFPD - FEDINA - MDC","{FF3500}Errore lettura reato.","Chiudi","");

    new result[200];
    //Variavbili Stringa [contenuto del db]
    new reato[200], poliziotto[MAX_PLAYER_NAME+1], data_ora[60];
    //Variabili intere
    new prigione, multa;

    new StrReato[900], line[512];

    mysql_fetch_row(line);
    mysql_get_field("reato", reato);
    mysql_get_field("multa", result); multa = strval(result);
    mysql_get_field("prigione", result); prigione  = strval(result);
    mysql_get_field("poliziotto", poliziotto);
    mysql_get_field("data_ora", data_ora);

    reato = FormattaStr(reato);

    format(StrReato,sizeof(StrReato),"{086FA1}--------------------\n", StrReato);
    format(StrReato, sizeof(StrReato), "%s{FF8900}Agente: {FFFFFF}%s\n{FF8900}Data & Ora: {FFFFFF}%s\n", StrReato, poliziotto, data_ora);
    if(prigione == 1) format(StrReato, sizeof(StrReato), "Arrestato\n\n%s{FF8900}Multa: {FFFFFF}%i$\n", StrReato, multa);
    else if(multa==0) format(StrReato, sizeof(StrReato), "Segnalato\n\n%s",StrReato);
    else format(StrReato, sizeof(StrReato), "Multato\n\n%s{FF8900}Multa: {FFFFFF}%i$\n", StrReato, multa);
    format(StrReato,sizeof(StrReato),"%s{086FA1}--------------------", StrReato);

    mysql_free_result();
    return ShowPlayerDialog(playerid,FEDINA_DIALOG3 ,DIALOG_STYLE_MSGBOX,"{086FA1}SFPD - FEDINA - MDC",StrReato,"Chiudi","");
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

forward GetFedina(playerid,suspectid);
public GetFedina(playerid,suspectid)
{
    IdSusAnalized[playerid] = suspectid;
    new line[512];

    new QueryFedina[150];
    new playername[MAX_PLAYER_NAME + 1];

    GetPlayerName(suspectid, playername, sizeof(playername));

    new tipo[25];
    if(SkipDialog[playerid]==0)
    {
        format(QueryFedina, sizeof(QueryFedina), "SELECT * FROM ricercati WHERE ricercato='%s'", playername);
        mysql_query(QueryFedina);
        mysql_store_result();
        
        if(mysql_num_rows()>0) 
        {
            new StrRicercato[900];
            SkipDialog[playerid]=1;

            mysql_fetch_row(line);

            new motivazione[200], poliziotto[MAX_PLAYER_NAME+1], data_ora[60];
            mysql_get_field("motivazione", motivazione);
            mysql_get_field("poliziotto", poliziotto);
            mysql_get_field("data_ora", data_ora);

            motivazione = FormattaStr(motivazione);

            mysql_free_result();

            format(StrRicercato,sizeof(StrRicercato),"Il soggetto � ricercato!\n\n{FF8900}Motivazione: {FFFFFF}%s\n\n{FF8900}Registrato da: {FFFFFF}%s\n{FF8900}Data & Ora: {FFFFFF}%s\n",motivazione, poliziotto, data_ora);
            return ShowPlayerDialog(playerid,FEDINA_DIALOG3 ,DIALOG_STYLE_MSGBOX,"{086FA1}SFPD - FEDINA - MDC",StrRicercato,"Chiudi","");
        }
        else mysql_free_result();




    }
    SkipDialog[playerid]=0;

    format(QueryFedina, sizeof(QueryFedina), "SELECT * FROM reati WHERE user='%s'", playername);
    mysql_query(QueryFedina);
    mysql_store_result();

    if(mysql_num_rows() == 0) return ShowPlayerDialog(playerid,FEDINA_DIALOG1 ,DIALOG_STYLE_MSGBOX,"{086FA1}SFPD - FEDINA - MDC","{00B64F}La fedina � pulita.","Chiudi","");

    new result[200];
    //Variavbili Stringa [contenuto del db]
    new reato[200];
    //Variabili intere
    new idreato, prigione, multa;

    new StrFedina[900];

    for(new i=0; i<mysql_num_rows(); i++)
    {
        mysql_fetch_row(line);
        mysql_get_field("id", result); idreato = strval(result);
        mysql_get_field("reato", reato);
        mysql_get_field("multa", result); multa = strval(result);
        mysql_get_field("prigione", result); prigione = strval(result);

        if(prigione==1) format(tipo, sizeof(tipo), "{FF3500}ARRESTATO");
        else
            if(multa==0) format(tipo, sizeof(tipo), "{086FA1}SEGNALATO");
            else format(tipo, sizeof(tipo), "{FF8900}MULTATO");

        reato = RiduciStr(reato);
        
        ListReati[playerid][i]=idreato;
        format(StrFedina,sizeof(StrFedina),"{FFFFFF}%s%s\t%s\n",StrFedina,reato,tipo);
    }
    mysql_free_result();
    format(StrFedina,sizeof(StrFedina),"REATO\tTIPO\n%s",StrFedina);
    return ShowPlayerDialog(playerid, FEDINA_DIALOG2, DIALOG_STYLE_TABLIST_HEADERS,"SFPD - FEDINA - MDC", StrFedina, "Select","Chiudi");
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//COMANDI
dcmd_fedina(playerid, params[])
{
    new suspectid;
	if(sscanf(params, "u", suspectid)) return SendClientMessage(playerid, COLOR_ERROR, "* /fedina [playerid]");
    if(suspectid == INVALID_PLAYER_ID || IsPlayerNPC(suspectid)) return SendClientMessage(playerid, COLOR_ERROR, "* Giocatore non trovato.");
	if(morto[playerid] == 1) return SendClientMessage(playerid, COLOR_ERROR, "* Sei in sistema morte.");
	if(PlayerInfo[playerid][pFaction] >= 2) return SendClientMessage(playerid, COLOR_ERROR, "* Non fai parte del Police Department!");
	if(!CopOnDuty[playerid]) return SendClientMessage(playerid, COLOR_ERROR, "* Non sei in servizio.");
    return GetFedina(playerid, suspectid);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid==FEDINA_DIALOG1){IdSusAnalized[playerid]=999;}

    if(dialogid==FEDINA_DIALOG2)
        if(response==1) return GetReatoData(listitem,playerid);
        else IdSusAnalized[playerid]=999;

    if(dialogid==FEDINA_DIALOG3) return GetFedina(playerid,IdSusAnalized[playerid]);
    
    //-------------------------------------------------------------------------
    #if defined fedina_OnDialogResponse
        return fedina_OnDialogResponse();
    #else
        return 1;
    #endif
}

#if defined _ALS_OnDialogResponse
    #undef OnDialogResponse
#else
    #define _ALS_OnDialogResponse
#endif

#define OnDialogResponse fedina_OnDialogResponse
#if defined fedina_OnDialogResponse
    forward fedina_OnDialogResponse();
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/* 
public OnPlayerCommandText(playerid, cmdtext[])
{
	if (strcmp("/fedina", cmdtext, true, 10) == 0)
	{
        new userID;
		if(sscanf(cmdtext, "u", userID))
		    return SendClientMessage(playerid, -1, "{FF3500}USA: {FFFFFF}/fedina [ID]");
        if(!IsPlayerConnected(userID))
		    return SendClientMessage(playerid, -1, "{FF8900}Quel giocatore non e' connesso.");
        return GetFedina(playerid,userID);
	}

    //-------------------------------------------------------------------------
    
    #if defined fedina_OnPlayerCommandText
        return fedina_OnPlayerCommandText();
    #else
        return 0;
    #endif
}

#if defined _ALS_OnPlayerCommandText
    #undef OnPlayerCommandText
#else
    #define _ALS_OnPlayerCommandText
#endif

#define OnPlayerCommandText fedina_OnPlayerCommandText
#if defined fedina_OnPlayerCommandText
    forward fedina_OnPlayerCommandText();
#endif
*/