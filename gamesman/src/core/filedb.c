#define DBVER 1 //Will be stored as a short, thus only integers.


/***********
************
**	Database functions.
**
**	Name: writeDatabase()
**
**	Description: writes gDatabase to a compressed file in gzip format.
**
**	Inputs: none
**
**	Outputs: none
**
**	Calls:	(In libz libraries)
**			gzopen 
**			gzclose
**			gzwrite
**			(In std libraries)
**			htonl
**			ntohl
**
**	Requierments:	gDatabase contains a valid database of positions
**					gNumberOfPositions stores the correct number of positions in gDatabase
**					kDBName is set correctly.
**					getOption() returns the correct option number
**					
**
**
**	Name: loadDatabase()
**
**	Description: loads the compressed file in gzip format into gDatabase.
**
**	Inputs: none
**
**	Outputs: none
**
**	Calls:	(In libz libraries)
**			gzopen 
**			gzclose
**			gzread
**			(In std libraries)
**			ntohl
**
**	Requierments:	gDatabase has enough space malloced to store uncompressed database
**					gNumberOfPositions stores the correct number of uncompressed positions in gDatabase
**					kDBName is set correctly.
**					getOption() returns the correct option number
**
**		~Scott
************
***********/
int writeDatabase()
{
    short dbVer[1];
    POSITION numPos[1];
    unsigned long i;
    gzFile * filep;
    char outfilename[256] ;
    int goodCompression = 1;
    int goodClose = 0;
    unsigned long tot = 0,sTot = gNumberOfPositions;
    
    if (gTwoBits)	/* TODO: Make db's compatible with 2-bits */
        return 0;	/* for some reason, 0 is error. -JJ */
    
    mkdir("data", 0755) ;
    sprintf(outfilename, "./data/m%s_%d.dat.gz", kDBName, getOption());
    if((filep = gzopen(outfilename, "wb")) == NULL) {
        if(kDebugDetermineValue){
            printf("Unable to create compressed data file\n");
        }
        return 0;
    }
    
    dbVer[0] = htons(DBVER);
    numPos[0] = htonl(gNumberOfPositions);
    goodCompression = gzwrite(filep, dbVer, sizeof(short));
    goodCompression = gzwrite(filep, numPos, sizeof(POSITION));
    for(i=0;i<gNumberOfPositions && goodCompression;i++){ //convert to network byteorder for platform independence.
        gDatabase[i] = htonl(gDatabase[i]);
        goodCompression = gzwrite(filep, gDatabase+i,sizeof(VALUE));
        tot += goodCompression;
        gDatabase[i] = ntohl(gDatabase[i]);
        //gzflush(filep,Z_FULL_FLUSH);
    }
    goodClose = gzclose(filep);
    
    if(goodCompression && (goodClose == 0))
	{
	    if(kDebugDetermineValue && ! gJustSolving){
		printf("File Successfully compressed\n");
	    }
	    return 1;
	} else {
        if(kDebugDetermineValue){
            fprintf(stderr, "\nError in file compression.\n Error codes:\ngzwrite error: %d\ngzclose error:%d\nBytes To Be Written: %u\nBytes Written:%u\n",goodCompression, goodClose,sTot*4,tot);
        }
        remove(outfilename);
        return 0;
	}
    
}

int loadDatabase()
{
    short dbVer[1];
    POSITION numPos[1];
    POSITION i;
    gzFile * filep ;
    char outfilename[256] ;
    int goodDecompression = 1;
    int goodClose = 1;
    unsigned long sTot = gNumberOfPositions;
    BOOLEAN correctDBVer;
    
    if (gTwoBits)	/* TODO: Same here */
        return 0;
    
    sprintf(outfilename, "./data/m%s_%d.dat.gz", kDBName, getOption()) ;
    if((filep = gzopen(outfilename, "rb")) == NULL) return 0 ;
    
    goodDecompression = gzread(filep,dbVer,sizeof(short));
    goodDecompression = gzread(filep,numPos,sizeof(POSITION));
    *dbVer = ntohs(*dbVer);
    *numPos = ntohl(*numPos);
    if(*numPos != gNumberOfPositions && kDebugDetermineValue){
        printf("\n\nError in file decompression: Stored gNumberOfPositions differs from internal gNumberOfPositions\n\n");
        return 0;
    }
    /***
     ** Database Ver. 1 Decompress
     ***/
    correctDBVer = (*dbVer == DBVER);
    
    if (correctDBVer) {
        for(i = 0; i < gNumberOfPositions && goodDecompression; i++){
            goodDecompression = gzread(filep, gDatabase+i, sizeof(VALUE));
            gDatabase[i] = ntohl(gDatabase[i]);
        }
    }
    /***
     ** End Ver. 1
     ***/
    
    
    goodClose = gzclose(filep);	
    

    if(goodDecompression && (goodClose == 0) && correctDBVer)
	{
	    if(kDebugDetermineValue){
            printf("File Successfully Decompressed\n");
	    }
	    return 1;
	}else{
	    for(i = 0 ; i < gNumberOfPositions ; i++)
		gDatabase[i] = undecided ;
	    if(kDebugDetermineValue){
		printf("\n\nError in file decompression:\ngzread error: %d\ngzclose error: %d\ndb version: %d\n",goodDecompression,goodClose,*dbVer);
	    }
	    return 0;
	}
    
}

/*************
**************
**
** End Database Code
**
**
**************
*************/