//2.53 Updated to 16th Jan, 2017
#include "../precompiled.h" //always first

#include "TypeChecker.h"
#include "ParseError.h"
#include "GlobalSymbols.h"
#include "../zsyssimple.h"
#include <assert.h>
#include <string>

//godawful template type, I'm sorry :-/
typedef pair<pair<TypeCheck *, pair<SymbolTable *, int> *>, int *> GLVT ;

////////////////////////////////////////////////////////////////
// TypeCheck
    
// Statements

void TypeCheck::caseStmtAssign(ASTStmtAssign &host, void *param)
{
    pair<SymbolTable *, int> *realp = (pair<SymbolTable *, int> *)param;
    //host.getLVal()->execute(*this, param);
    host.getRVal()->execute(*this, param);
    
    if(failure)
        return;
        
    int ltype;
    GLVT p = GLVT(pair<TypeCheck *, pair<SymbolTable *, int> *>(this,realp), &ltype);
    GetLValType temp;
    host.getLVal()->execute(temp, &p);
        
    if(failure)
            return;
            
    int rtype = host.getRVal()->getType();
    
    if(!standardCheck(ltype, rtype, &host))
        {
        failure = true;
        }
    }
    
void TypeCheck::caseStmtIf(ASTStmtIf &host, void *param)
{
    RecursiveVisitor::caseStmtIf(host, param);
    
    if(failure)
        return;
        
    int type = host.getCondition()->getType();
    
    if(!standardCheck(ScriptParser::TYPE_BOOL, type, &host))
    {
        failure = true;
        return;
    }
}
        
void TypeCheck::caseStmtIfElse(ASTStmtIfElse &host, void *param)
{
    caseStmtIf(host, param);
    host.getElseStmt()->execute(*this,param);
}
        
void TypeCheck::caseStmtSwitch(ASTStmtSwitch &host, void* param)
{
	RecursiveVisitor::caseStmtSwitch(host, param);
	if (failure) return;

	int key_type = host.getKey()->getType();
	if (!standardCheck(ScriptParser::TYPE_FLOAT, key_type, &host))
	{
		failure = true;
		return;
	}
}

void caseSwitchCases(ASTSwitchCases &host, void* param);

void TypeCheck::caseStmtFor(ASTStmtFor &host, void *param)
{
    RecursiveVisitor::caseStmtFor(host, param);
        
    if(failure)
        return;
        
    int type = host.getTerminationCondition()->getType();
        
    if(!standardCheck(ScriptParser::TYPE_BOOL, type, &host))
    {
        failure = true;
        return;
    }
}
        
void TypeCheck::caseStmtWhile(ASTStmtWhile &host, void *param)
{
    RecursiveVisitor::caseStmtWhile(host, param);
        
    if(failure)
        return;
        
    int type = host.getCond()->getType();
    
    if(!standardCheck(ScriptParser::TYPE_BOOL, type, &host))
    {
        failure = true;
        return;
    }
}
    
void TypeCheck::caseStmtReturn(ASTStmtReturn &host, void *param)
    {
    int rettype = ((pair<SymbolTable *, int> *)param)->second;
    
    if(rettype != ScriptParser::TYPE_VOID)
    {
        printErrorMsg(&host, FUNCBADRETURN, ScriptParser::printType(rettype));
        failure = true;
    }
}

void TypeCheck::caseStmtReturnVal(ASTStmtReturnVal &host, void *param)
{
    host.getReturnValue()->execute(*this, param);
    
    if(failure)
        return;
    
    int type = host.getReturnValue()->getType();
    int rettype = ((pair<SymbolTable *, int> *)param)->second;
    
    if(!standardCheck(rettype, type, &host))
    {
        failure = true;
    }
}

// Declarations

void TypeCheck::caseArrayDecl(ASTArrayDecl &host, void *param)
{
	ASTExpr *size = host.getSize();

	size->execute(*this, param);
    
	if (size->getType() != ScriptParser::TYPE_FLOAT)
    {
            printErrorMsg(&host, NONINTEGERARRAYSIZE, "");
            failure = true;
        return;
    }
    
    SymbolTable * st = ((pair<SymbolTable *, int>*) param)->first;
    int arraytype = st->getVarType(&host);
    
    if(host.getList() != NULL)
    {
        list<ASTExpr *> l = host.getList()->getList();
        
        for(list<ASTExpr *>::iterator it = l.begin(); it != l.end(); it++)
        {
            (*it)->execute(*this, param);
            
            if(failure)
            return;
            
            if(!standardCheck(arraytype, (*it)->getType(), &host))
        {
                failure = true;
            return;
        }
    }
}
}

void TypeCheck::caseVarDeclInitializer(ASTVarDeclInitializer &host, void *param)
        {
    SymbolTable * st = ((pair<SymbolTable *, int> *)param)->first;
    ASTExpr *init = host.getInitializer();
    init->execute(*this, param);
            
    if(failure)
        return;
            
    int type = init->getType();
    int ltype = st->getVarType(&host);
                
    if(!standardCheck(ltype, type, &host))
    {
        failure = true;
        }
    }
    
// Expressions

void TypeCheck::caseExprConst(ASTExprConst &host, void *param)
{
	ASTExpr *content = host.getContent();
	content->execute(*this, param);

	if (!host.isConstant())
	{
		failure = true;
        printErrorMsg(&host, EXPRNOTCONSTANT);
		return;
	}

	host.setType(content->getType());
	if (content->hasIntValue())
		host.setIntValue(content->getIntValue());
}

void TypeCheck::caseNumConstant(ASTNumConstant &host, void *)
        {
    host.setType(ScriptParser::TYPE_FLOAT);
    pair<string,string> parts = host.getValue()->parseValue();
    pair<long, bool> val = ScriptParser::parseLong(parts);
    
    if(!val.second)
        {
        printErrorMsg(&host, CONSTTRUNC, host.getValue()->getValue());
    }
                
    host.setIntValue(val.first);
        }
        
void TypeCheck::caseBoolConstant(ASTBoolConstant &host, void *)
{
    host.setType(ScriptParser::TYPE_BOOL);
    host.setIntValue(host.getValue() ? 1 : 0);
}
            
void TypeCheck::caseStringConstant(ASTStringConstant& host, void*)
{
	host.setType(ScriptParser::TYPE_FLOAT);
}

void TypeCheck::caseExprDot(ASTExprDot &host, void *param)
        {
    SymbolTable *st = ((pair<SymbolTable *, int> *)param)->first;
                
    if(st->isConstant(host.getName()))
	{
        host.setType(ScriptParser::TYPE_FLOAT);
		host.setIntValue(st->getConstantVal(host.getName()));
	}
    else
    {
        int type  = st->getVarType(&host);
        host.setType(type);
        }
    }
    
void TypeCheck::caseExprArrow(ASTExprArrow &host, void *param)
    {
    SymbolTable *st = ((pair<SymbolTable *, int> *)param)->first;
    //annoyingly enough I have to treat arrowed variables as function calls
    //get the left-hand type
    host.getLVal()->execute(*this,param);
    
    if(failure)
        return;
        
    bool isIndexed = (host.getIndex() != NULL);
    
    if(isIndexed)
    {
        host.getIndex()->execute(*this,param);
            
        if(failure)
            return;
            
        if(!standardCheck(ScriptParser::TYPE_FLOAT, host.getIndex()->getType(), host.getIndex()))
        {
            failure = true;
            return;
        }
    }
    
    int type = host.getLVal()->getType();
    pair<int, vector<int> > fidparam;
    string name = "get" + host.getName();
            
    if(isIndexed)
        name += "[]";
            
    switch(type)
    {
    case ScriptParser::TYPE_FFC:
    {
        fidparam = FFCSymbols::getInst().matchFunction(name,st);
        break;
    }
    
    case ScriptParser::TYPE_LINK:
    {
        fidparam = LinkSymbols::getInst().matchFunction(name,st);
        break;
    }
            
    case ScriptParser::TYPE_SCREEN:
    {
        fidparam = ScreenSymbols::getInst().matchFunction(name, st);
        break;
    }
    
    case ScriptParser::TYPE_GAME:
    {
        fidparam = GameSymbols::getInst().matchFunction(name, st);
        break;
    }
    
    case ScriptParser::TYPE_ITEM:
    {
        fidparam = ItemSymbols::getInst().matchFunction(name, st);
        break;
    }
    
    case ScriptParser::TYPE_ITEMCLASS:
    {
        fidparam = ItemclassSymbols::getInst().matchFunction(name, st);
        break;
    }
    
    case ScriptParser::TYPE_NPC:
    {
        fidparam = NPCSymbols::getInst().matchFunction(name,st);
        break;
    }
    
    case ScriptParser::TYPE_LWPN:
    {
        fidparam = LinkWeaponSymbols::getInst().matchFunction(name,st);
        break;
    }
    
    case ScriptParser::TYPE_EWPN:
    {
        fidparam = EnemyWeaponSymbols::getInst().matchFunction(name,st);
        break;
    }
    case ScriptParser::TYPE_NPCDATA:
    {
        fidparam = NPCDataSymbols::getInst().matchFunction(name,st);
        break;
    }
    case ScriptParser::TYPE_DEBUG:
    {
        fidparam = DebugSymbols::getInst().matchFunction(name,st);
        break;
    }
    case ScriptParser::TYPE_AUDIO:
    {
        fidparam = AudioSymbols::getInst().matchFunction(name,st);
        break;
    }
    case ScriptParser::TYPE_COMBOS:
    {
        fidparam = CombosPtrSymbols::getInst().matchFunction(name,st);
        break;
    }
    case ScriptParser::TYPE_SPRITEDATA:
    {
        fidparam = SpriteDataSymbols::getInst().matchFunction(name,st);
        break;
    }
    case ScriptParser::TYPE_GRAPHICS:
    {
        fidparam = GfxPtrSymbols::getInst().matchFunction(name,st);
        break;
    }
    case ScriptParser::TYPE_TEXT:
    {
        fidparam = TextPtrSymbols::getInst().matchFunction(name,st);
        break;
    }
    case ScriptParser::TYPE_INPUT:
    {
        fidparam = InputSymbols::getInst().matchFunction(name,st);
        break;
    }
    case ScriptParser::TYPE_MAPDATA:
    {
        fidparam = MapDataSymbols::getInst().matchFunction(name,st);
        break;
    }
    
    case ScriptParser::TYPE_DMAPDATA:
    {
        fidparam = DMapDataSymbols::getInst().matchFunction(name,st);
        break;
    }
    
    case ScriptParser::TYPE_SHOPDATA:
    {
        fidparam = ShopDataSymbols::getInst().matchFunction(name,st);
        break;
    }
    case ScriptParser::TYPE_ZMESSAGE:
    {
        fidparam = MessageDataSymbols::getInst().matchFunction(name,st);
        break;
    }
    default:
        failure = true;
        printErrorMsg(&host, ARROWNOTPOINTER);
        return;
}

    if(fidparam.first == -1 || (int)fidparam.second.size() != (isIndexed ? 2 : 1) || fidparam.second[0] != type)
{
        failure = true;
        printErrorMsg(&host, ARROWNOVAR, host.getName() + (isIndexed ? "[]" : ""));
        return;
}

    st->putAST(&host, fidparam.first);
    host.setType(st->getFuncType(fidparam.first));
}

void TypeCheck::caseExprArray(ASTExprArray &host, void *param)
{
    SymbolTable * st = ((pair<SymbolTable *, int> *)param)->first;
        
    int type  = st->getVarType(&host);
    host.setType(type);
    
    if(host.getIndex())
    {
        host.getIndex()->execute(*this,param);

        if(failure)
            return;
        
        if(!standardCheck(ScriptParser::TYPE_FLOAT, host.getIndex()->getType(), host.getIndex()))
        {
            failure = true;
            return;
        }
    }
}
    
void TypeCheck::caseFuncCall(ASTFuncCall &host, void *param)
    {
    //yuck. Time to disambiguate these damn functions
    //build the param types
    list<ASTExpr *> params = host.getParams();
    vector<int> paramtypes;
    vector<int> possible;
    bool isdotexpr;
    IsDotExpr temp;
    host.getName()->execute(temp, &isdotexpr);
        
    //if this is a simple function, we already have what we need
    //otherwise we need the type of the thing being arrowed
    if (!isdotexpr)
    {
        ASTExprArrow *lval = (ASTExprArrow *)host.getName();
        lval->getLVal()->execute(*this,param);
            
            if(failure)
                return;
                
        int lvaltype = lval->getLVal()->getType();
        
        if(!(lvaltype == ScriptParser::TYPE_FFC
             || lvaltype == ScriptParser::TYPE_LINK
             || lvaltype == ScriptParser::TYPE_SCREEN
             || lvaltype == ScriptParser::TYPE_ITEM
             || lvaltype == ScriptParser::TYPE_ITEMCLASS
             || lvaltype == ScriptParser::TYPE_GAME
             || lvaltype == ScriptParser::TYPE_NPC
             || lvaltype == ScriptParser::TYPE_LWPN
             || lvaltype == ScriptParser::TYPE_EWPN
             || lvaltype == ScriptParser::TYPE_NPCDATA
             || lvaltype == ScriptParser::TYPE_DEBUG
             || lvaltype == ScriptParser::TYPE_AUDIO
             || lvaltype == ScriptParser::TYPE_COMBOS
             || lvaltype == ScriptParser::TYPE_SPRITEDATA
             || lvaltype == ScriptParser::TYPE_GRAPHICS
             || lvaltype == ScriptParser::TYPE_TEXT
             || lvaltype == ScriptParser::TYPE_INPUT
             || lvaltype == ScriptParser::TYPE_MAPDATA
             || lvaltype == ScriptParser::TYPE_DMAPDATA
             || lvaltype == ScriptParser::TYPE_ZMESSAGE
             || lvaltype == ScriptParser::TYPE_SHOPDATA 
             || lvaltype == ScriptParser::TYPE_UNTYPED
	     
             || lvaltype == ScriptParser::TYPE_DROPSET
             || lvaltype == ScriptParser::TYPE_PONDS
             || lvaltype == ScriptParser::TYPE_WARPRING
             || lvaltype == ScriptParser::TYPE_DOORSET
             || lvaltype == ScriptParser::TYPE_ZUICOLOURS
             || lvaltype == ScriptParser::TYPE_RGBDATA
             || lvaltype == ScriptParser::TYPE_PALETTE
             || lvaltype == ScriptParser::TYPE_TUNES
             || lvaltype == ScriptParser::TYPE_PALCYCLE
             || lvaltype == ScriptParser::TYPE_GAMEDATA
             || lvaltype == ScriptParser::TYPE_CHEATS
	     
	     ))
        {
	        printErrorMsg(lval, ARROWNOTPOINTER);
	        failure = true;
	        return;
        }
        
        //prepend that type to the function parameters, as that is implicitly passed
        paramtypes.push_back(lvaltype);
    }

    //now add the normal parameters
    for(list<ASTExpr *>::iterator it = params.begin(); it != params.end(); it++)
    {
        (*it)->execute(*this, param);
    
        if(failure)
	        return;
        
        paramtypes.push_back((*it)->getType());
    }
    
    SymbolTable *st = ((pair<SymbolTable *, int> *)param)->first;
    string paramstring = "(";
    bool firsttype = true;
    
    for(vector<int>::iterator it = paramtypes.begin(); it != paramtypes.end(); it++)
    {
        if(firsttype)
            firsttype = false;
        else
            paramstring += ", ";

        paramstring += ScriptParser::printType(*it);
    }
    
    paramstring += ")";
        
    if(isdotexpr)
    {
        possible = st->getAmbiguousFuncs(&host);
    
    
        vector<pair<int, int> > matchedfuncs;
        
        for(vector<int>::iterator it = possible.begin(); it != possible.end(); it++)
        {
            vector<int> stparams = st->getFuncParams(*it);
        
            //see if they match
            if(stparams.size() != paramtypes.size())
                continue;

            bool matched = true;
            int diffs = 0;
            
            for(unsigned int i=0; i<stparams.size(); i++)
            {
                if(!standardCheck(stparams[i],paramtypes[i], NULL))
                {
                    matched=false;
                    break;
                }
    
                if(stparams[i] != paramtypes[i])
                    diffs++;
            }
        
            if(matched)
            {
                matchedfuncs.push_back(pair<int,int>(*it, diffs));
            }
        }
    
        //now find the closest match *sigh*
        vector<int> bestmatch;
        int bestdiffs = 10000;
    
        for(vector<pair<int, int> >::iterator it = matchedfuncs.begin(); it != matchedfuncs.end(); it++)
        {
            if((*it).second < bestdiffs)
            {
                bestdiffs = (*it).second;
                bestmatch.clear();
                bestmatch.push_back((*it).first);
            }
            else if((*it).second == bestdiffs)
            {
                bestmatch.push_back((*it).first);
            }
        }

        string fullname;
    
        if(((ASTExprDot *)host.getName())->getNamespace() == "")
            fullname = ((ASTExprDot*)host.getName())->getName();
        else
            fullname = ((ASTExprDot *)host.getName())->getNamespace() + "." + ((ASTExprDot *)host.getName())->getName();
        
        if(bestmatch.size() == 0)
        {
            printErrorMsg(&host, NOFUNCMATCH, fullname + paramstring);
            failure = true;
            return;
        }
        else if(bestmatch.size() > 1)
        {
            printErrorMsg(&host, TOOFUNCMATCH, fullname+paramstring);
            failure = true;
            return;
        }
        
        //WHEW!
        host.setType(st->getFuncType(bestmatch[0]));
        st->putAST(&host, bestmatch[0]);
    }
    else
    {
        //still have to deal with the (%&# arrow functions
        //luckily I will here assert that each type's functions MUST be unique
        ASTExprArrow *name = (ASTExprArrow *)host.getName();
        int type = name->getLVal()->getType();
        pair<int, vector<int> > fidtype;

        switch(type)
        {
        case ScriptParser::TYPE_FFC:
            fidtype = FFCSymbols::getInst().matchFunction(name->getName(),st);
            break;
    
        case ScriptParser::TYPE_LINK:
            fidtype = LinkSymbols::getInst().matchFunction(name->getName(),st);
            break;
        
        case ScriptParser::TYPE_SCREEN:
            fidtype = ScreenSymbols::getInst().matchFunction(name->getName(),st);
            break;
    
        case ScriptParser::TYPE_GAME:
            fidtype = GameSymbols::getInst().matchFunction(name->getName(),st);
            break;
    
        case ScriptParser::TYPE_ITEM:
            fidtype = ItemSymbols::getInst().matchFunction(name->getName(),st);
            break;
        
        case ScriptParser::TYPE_ITEMCLASS:
            fidtype = ItemclassSymbols::getInst().matchFunction(name->getName(), st);
            break;
            
        case ScriptParser::TYPE_NPC:
            fidtype = NPCSymbols::getInst().matchFunction(name->getName(), st);
            break;
            
        case ScriptParser::TYPE_LWPN:
            fidtype = LinkWeaponSymbols::getInst().matchFunction(name->getName(), st);
            break;
            
        case ScriptParser::TYPE_EWPN:
            fidtype = EnemyWeaponSymbols::getInst().matchFunction(name->getName(), st);
            break;
            
        case ScriptParser::TYPE_NPCDATA:
            fidtype = NPCDataSymbols::getInst().matchFunction(name->getName(), st);
            break;

        case ScriptParser::TYPE_MAPDATA:
            fidtype = MapDataSymbols::getInst().matchFunction(name->getName(), st);
            break;

        case ScriptParser::TYPE_DEBUG:
            fidtype = DebugSymbols::getInst().matchFunction(name->getName(), st);
            break;

        case ScriptParser::TYPE_AUDIO:
            fidtype = AudioSymbols::getInst().matchFunction(name->getName(), st);
            break;

        case ScriptParser::TYPE_COMBOS:
            fidtype = CombosPtrSymbols::getInst().matchFunction(name->getName(), st);
            break;

        case ScriptParser::TYPE_SPRITEDATA:
            fidtype = SpriteDataSymbols::getInst().matchFunction(name->getName(), st);
            break;

        case ScriptParser::TYPE_GRAPHICS:
            fidtype = GfxPtrSymbols::getInst().matchFunction(name->getName(), st);
            break;

        case ScriptParser::TYPE_TEXT:
            fidtype = TextPtrSymbols::getInst().matchFunction(name->getName(), st);
            break;
	
        case ScriptParser::TYPE_INPUT:
            fidtype = InputSymbols::getInst().matchFunction(name->getName(), st);
            break;
            
        case ScriptParser::TYPE_DMAPDATA:
	        fidtype = DMapDataSymbols::getInst().matchFunction(name->getName(), st);
	        break;

        case ScriptParser::TYPE_ZMESSAGE:
	        fidtype = MessageDataSymbols::getInst().matchFunction(name->getName(), st);
	        break;

        case ScriptParser::TYPE_SHOPDATA:
	        fidtype = ShopDataSymbols::getInst().matchFunction(name->getName(), st);
	        break;
	
        case ScriptParser::TYPE_UNTYPED:
	        fidtype = UntypedSymbols::getInst().matchFunction(name->getName(), st);
	        break;
	case ScriptParser::TYPE_DROPSET:
	        fidtype = DropsetSymbols::getInst().matchFunction(name->getName(), st);
	        break;
	case ScriptParser::TYPE_PONDS:
	        fidtype = PondSymbols::getInst().matchFunction(name->getName(), st);
	        break;
	case ScriptParser::TYPE_WARPRING:
	        fidtype = WarpringSymbols::getInst().matchFunction(name->getName(), st);
	        break;
	case ScriptParser::TYPE_DOORSET:
	        fidtype = DoorsetSymbols::getInst().matchFunction(name->getName(), st);
	        break;
	case ScriptParser::TYPE_ZUICOLOURS:
	        fidtype = MiscColourSymbols::getInst().matchFunction(name->getName(), st);
	        break;
	case ScriptParser::TYPE_RGBDATA:
	        fidtype = RGBSymbols::getInst().matchFunction(name->getName(), st);
	        break;
	case ScriptParser::TYPE_PALETTE:
	        fidtype = PaletteSymbols::getInst().matchFunction(name->getName(), st);
	        break;
	case ScriptParser::TYPE_TUNES:
	        fidtype = TunesSymbols::getInst().matchFunction(name->getName(), st);
	        break;
	case ScriptParser::TYPE_PALCYCLE:
	        fidtype = PalCycleSymbols::getInst().matchFunction(name->getName(), st);
	        break;
	case ScriptParser::TYPE_GAMEDATA:
	        fidtype = GamedataSymbols::getInst().matchFunction(name->getName(), st);
	        break;
	case ScriptParser::TYPE_CHEATS:
	        fidtype = CheatsSymbols::getInst().matchFunction(name->getName(), st);
	        break;

        default:
            assert(false);
        }

        if(fidtype.first == -1)
        {
            failure = true;
            printErrorMsg(&host, ARROWNOFUNC, name->getName());
            return;
        }
        
        if(paramtypes.size() != fidtype.second.size())
        {
	        failure = true;
            printErrorMsg(&host, NOFUNCMATCH, name->getName() + paramstring);
            return;
        }
    
        for(unsigned int i=0; i<paramtypes.size(); i++)
    {
            if(!standardCheck(fidtype.second[i], paramtypes[i],NULL))
            {
                failure = true;
                printErrorMsg(&host, NOFUNCMATCH, name->getName() + paramstring);
                return;
            }
        }
        
        host.setType(st->getFuncType(fidtype.first));
        st->putAST(&host, fidtype.first);
    }
}

void TypeCheck::caseExprNegate(ASTExprNegate &host, void *param)
{
    host.getOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    if(!standardCheck(ScriptParser::TYPE_FLOAT, host.getOperand()->getType(), &host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_FLOAT);
    
    if(host.getOperand()->hasIntValue())
    {
        long val = host.getOperand()->getIntValue();
        host.setIntValue(-val);
    }
}

void TypeCheck::caseExprNot(ASTExprNot &host, void *param)
{
    host.getOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    if(!standardCheck(ScriptParser::TYPE_BOOL, host.getOperand()->getType(), &host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_BOOL);
    
    if(host.getOperand()->hasIntValue())
    {
        long val = host.getOperand()->getIntValue();
        
        if(val == 0)
            host.setIntValue(1);
        else
            host.setIntValue(0);
    }
}

void TypeCheck::caseExprBitNot(ASTExprBitNot &host, void *param)
{
    host.getOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    if(!standardCheck(ScriptParser::TYPE_FLOAT, host.getOperand()->getType(), &host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_FLOAT);
    
    if(host.getOperand()->hasIntValue())
    {
        long val = host.getOperand()->getIntValue();
        host.setIntValue((~(val/10000))*10000);
    }
}

void TypeCheck::caseExprIncrement(ASTExprIncrement &host, void *param)
{
    pair<SymbolTable *, int> *realp = (pair<SymbolTable *, int> *)param;
    int type;
    GLVT p = GLVT(pair<TypeCheck *, pair<SymbolTable *, int> *>(this,realp),&type);
    host.getOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    bool isexprdot;
    IsDotExpr temp;
    host.getOperand()->execute(temp, &isexprdot);
    
    if(!isexprdot)
    {
        int fid = realp->first->getID(host.getOperand());
        realp->first->putAST(&host, fid);
}

    GetLValType glvt;
    host.getOperand()->execute(glvt, &p);
    
    if(failure)
        return;
        
    if(!standardCheck(ScriptParser::TYPE_FLOAT, *p.second, &host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_FLOAT);
}
    
void TypeCheck::caseExprPreIncrement(ASTExprPreIncrement &host, void *param)
    {
    pair<SymbolTable *, int> *realp = (pair<SymbolTable *, int> *)param;
    int type;
    GLVT p = GLVT(pair<TypeCheck *, pair<SymbolTable *, int> *>(this,realp),&type);
    host.getOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    bool isexprdot;
    IsDotExpr temp;
    host.getOperand()->execute(temp, &isexprdot);
    
    if(!isexprdot)
    {
        int fid = realp->first->getID(host.getOperand());
        realp->first->putAST(&host, fid);
    }
    
    GetLValType glvt;
    host.getOperand()->execute(glvt, &p);
    
    if(failure)
        return;
        
    if(!standardCheck(ScriptParser::TYPE_FLOAT, *p.second, &host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_FLOAT);
}

void TypeCheck::caseExprDecrement(ASTExprDecrement &host, void *param)
{
    pair<SymbolTable *, int> *realp = (pair<SymbolTable *, int> *)param;
    int type;
    GLVT p = GLVT(pair<TypeCheck *, pair<SymbolTable *, int> *>(this,realp),&type);
    host.getOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    bool isexprdot;
    IsDotExpr temp;
    host.getOperand()->execute(temp, &isexprdot);
    
    if(!isexprdot)
    {
        int fid = realp->first->getID(host.getOperand());
        realp->first->putAST(&host, fid);
    }
    
    GetLValType glvt;
    host.getOperand()->execute(glvt, &p);
    
    if(failure)
        return;
        
    if(!standardCheck(ScriptParser::TYPE_FLOAT, *p.second, &host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_FLOAT);
}
    
void TypeCheck::caseExprPreDecrement(ASTExprPreDecrement &host, void *param)
    {
    pair<SymbolTable *, int> *realp = (pair<SymbolTable *, int> *)param;
    int type;
    GLVT p = GLVT(pair<TypeCheck *, pair<SymbolTable *, int> *>(this,realp),&type);
    host.getOperand()->execute(*this, param);
        
    if(failure)
        return;
        
    bool isexprdot;
    IsDotExpr temp;
    host.getOperand()->execute(temp, &isexprdot);
    
    if(!isexprdot)
    {
        int fid = realp->first->getID(host.getOperand());
        realp->first->putAST(&host, fid);
    }
    
    GetLValType glvt;
    host.getOperand()->execute(glvt, &p);
    
    if(failure)
        return;
        
    if(!standardCheck(ScriptParser::TYPE_FLOAT, *p.second, &host))
        {
            failure = true;
            return;
        }
        
    host.setType(ScriptParser::TYPE_FLOAT);
}

void TypeCheck::caseExprAnd(ASTExprAnd &host, void *param)
{
    host.getFirstOperand()->execute(*this, param);
    host.getSecondOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    if(!standardCheck(ScriptParser::TYPE_BOOL, host.getFirstOperand()->getType(), &host)
            || !standardCheck(ScriptParser::TYPE_BOOL, host.getSecondOperand()->getType(), &host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_BOOL);
    
    if(host.getFirstOperand()->hasIntValue() && host.getSecondOperand()->hasIntValue())
    {
        long firstval = host.getFirstOperand()->getIntValue();
        long secondval = host.getSecondOperand()->getIntValue();
        
        if(firstval == 0 || secondval == 0)
            host.setIntValue(0);
        else
            host.setIntValue(1);
    }
}

void TypeCheck::caseExprOr(ASTExprOr &host, void *param)
{
    host.getFirstOperand()->execute(*this, param);
    host.getSecondOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    if(!standardCheck(ScriptParser::TYPE_BOOL, host.getFirstOperand()->getType(), &host)
            || !standardCheck(ScriptParser::TYPE_BOOL, host.getSecondOperand()->getType(),&host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_BOOL);
    
    if(host.getFirstOperand()->hasIntValue() && host.getSecondOperand()->hasIntValue())
    {
        long firstval = host.getFirstOperand()->getIntValue();
        long secondval = host.getSecondOperand()->getIntValue();
        
        if(firstval == 0 && secondval == 0)
            host.setIntValue(0);
        else
            host.setIntValue(1);
    }
}

void TypeCheck::caseExprGT(ASTExprGT &host, void *param)
{
    host.getFirstOperand()->execute(*this, param);
    host.getSecondOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    if(!standardCheck(ScriptParser::TYPE_FLOAT, host.getFirstOperand()->getType(), &host)
            || !standardCheck(ScriptParser::TYPE_FLOAT, host.getSecondOperand()->getType(),&host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_BOOL);
    
    if(host.getFirstOperand()->hasIntValue() && host.getSecondOperand()->hasIntValue())
    {
        long firstval = host.getFirstOperand()->getIntValue();
        long secondval = host.getSecondOperand()->getIntValue();
        
        if(firstval > secondval)
            host.setIntValue(1);
        else
            host.setIntValue(0);
    }
}

void TypeCheck::caseExprGE(ASTExprGE &host, void *param)
{
    host.getFirstOperand()->execute(*this, param);
    host.getSecondOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    if(!standardCheck(ScriptParser::TYPE_FLOAT, host.getFirstOperand()->getType(), &host)
            || !standardCheck(ScriptParser::TYPE_FLOAT, host.getSecondOperand()->getType(),&host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_BOOL);
    
    if(host.getFirstOperand()->hasIntValue() && host.getSecondOperand()->hasIntValue())
    {
        long firstval = host.getFirstOperand()->getIntValue();
        long secondval = host.getSecondOperand()->getIntValue();
        
        if(firstval >= secondval)
            host.setIntValue(1);
        else
            host.setIntValue(0);
    }
}

void TypeCheck::caseExprLT(ASTExprLT &host, void *param)
{
    host.getFirstOperand()->execute(*this, param);
    host.getSecondOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    if(!standardCheck(ScriptParser::TYPE_FLOAT, host.getFirstOperand()->getType(), &host)
            || !standardCheck(ScriptParser::TYPE_FLOAT, host.getSecondOperand()->getType(),&host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_BOOL);
    
    if(host.getFirstOperand()->hasIntValue() && host.getSecondOperand()->hasIntValue())
    {
        long firstval = host.getFirstOperand()->getIntValue();
        long secondval = host.getSecondOperand()->getIntValue();
        
        if(firstval < secondval)
            host.setIntValue(1);
        else
            host.setIntValue(0);
    }
}

void TypeCheck::caseExprLE(ASTExprLE &host, void *param)
{
    host.getFirstOperand()->execute(*this, param);
    host.getSecondOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    if(!standardCheck(ScriptParser::TYPE_FLOAT, host.getFirstOperand()->getType(), &host)
            || !standardCheck(ScriptParser::TYPE_FLOAT, host.getSecondOperand()->getType(),&host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_BOOL);
    
    if(host.getFirstOperand()->hasIntValue() && host.getSecondOperand()->hasIntValue())
    {
        long firstval = host.getFirstOperand()->getIntValue();
        long secondval = host.getSecondOperand()->getIntValue();
        
        if(firstval <= secondval)
            host.setIntValue(1);
        else
            host.setIntValue(0);
    }
}

void TypeCheck::caseExprEQ(ASTExprEQ &host, void *param)
{
    host.getFirstOperand()->execute(*this, param);
    host.getSecondOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    if(!standardCheck(host.getFirstOperand()->getType(), host.getSecondOperand()->getType(),
                      &host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_BOOL);
    
    if(host.getFirstOperand()->hasIntValue() && host.getSecondOperand()->hasIntValue())
    {
        long firstval = host.getFirstOperand()->getIntValue();
        long secondval = host.getSecondOperand()->getIntValue();
        
        if(firstval == secondval)
            host.setIntValue(1);
        else
            host.setIntValue(0);
    }
}

void TypeCheck::caseExprNE(ASTExprNE &host, void *param)
{
    host.getFirstOperand()->execute(*this, param);
    host.getSecondOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    if(!standardCheck(host.getFirstOperand()->getType(), host.getSecondOperand()->getType(),
                      &host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_BOOL);
    
    if(host.getFirstOperand()->hasIntValue() && host.getSecondOperand()->hasIntValue())
    {
        long firstval = host.getFirstOperand()->getIntValue();
        long secondval = host.getSecondOperand()->getIntValue();
        
        if(firstval != secondval)
            host.setIntValue(1);
        else
            host.setIntValue(0);
    }
}

void TypeCheck::caseExprPlus(ASTExprPlus &host, void *param)
{
    host.getFirstOperand()->execute(*this, param);
    host.getSecondOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    if(!standardCheck(ScriptParser::TYPE_FLOAT, host.getFirstOperand()->getType(), &host)
            || !standardCheck(ScriptParser::TYPE_FLOAT, host.getSecondOperand()->getType(),&host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_FLOAT);
    
    if(host.getFirstOperand()->hasIntValue() && host.getSecondOperand()->hasIntValue())
    {
        long firstval = host.getFirstOperand()->getIntValue();
        long secondval = host.getSecondOperand()->getIntValue();
        host.setIntValue(firstval+secondval);
    }
}

void TypeCheck::caseExprMinus(ASTExprMinus &host, void *param)
{
    host.getFirstOperand()->execute(*this, param);
    host.getSecondOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    if(!standardCheck(ScriptParser::TYPE_FLOAT, host.getFirstOperand()->getType(), &host)
            || !standardCheck(ScriptParser::TYPE_FLOAT, host.getSecondOperand()->getType(),&host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_FLOAT);
    
    if(host.getFirstOperand()->hasIntValue() && host.getSecondOperand()->hasIntValue())
    {
        long firstval = host.getFirstOperand()->getIntValue();
        long secondval = host.getSecondOperand()->getIntValue();
        host.setIntValue(firstval-secondval);
    }
    }
    
void TypeCheck::caseExprTimes(ASTExprTimes &host, void *param)
{
    host.getFirstOperand()->execute(*this, param);
    host.getSecondOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    if(!standardCheck(ScriptParser::TYPE_FLOAT, host.getFirstOperand()->getType(), &host)
            || !standardCheck(ScriptParser::TYPE_FLOAT, host.getSecondOperand()->getType(),&host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_FLOAT);
    
    if(host.getFirstOperand()->hasIntValue() && host.getSecondOperand()->hasIntValue())
    {
        long firstval = host.getFirstOperand()->getIntValue();
        long secondval = host.getSecondOperand()->getIntValue();
        double temp = ((double)secondval)/10000.0;
        host.setIntValue((long)(firstval*temp));
}
}

void TypeCheck::caseExprDivide(ASTExprDivide &host, void *param)
{
    host.getFirstOperand()->execute(*this, param);
    host.getSecondOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    if(!standardCheck(ScriptParser::TYPE_FLOAT, host.getFirstOperand()->getType(), &host)
            || !standardCheck(ScriptParser::TYPE_FLOAT, host.getSecondOperand()->getType(),&host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_FLOAT);
    
    if(host.getFirstOperand()->hasIntValue() && host.getSecondOperand()->hasIntValue())
    {
        long firstval = host.getFirstOperand()->getIntValue();
        long secondval = host.getSecondOperand()->getIntValue();
        
        if(secondval == 0)
    {
            printErrorMsg(&host, DIVBYZERO);
        failure = true;
        return;
    }
    
        host.setIntValue((firstval/secondval)*10000);
}
}

void TypeCheck::caseExprModulo(ASTExprModulo &host, void *param)
{
    host.getFirstOperand()->execute(*this, param);
    host.getSecondOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    if(!standardCheck(ScriptParser::TYPE_FLOAT, host.getFirstOperand()->getType(), &host)
            || !standardCheck(ScriptParser::TYPE_FLOAT, host.getSecondOperand()->getType(),&host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_FLOAT);
    
    if(host.getFirstOperand()->hasIntValue() && host.getSecondOperand()->hasIntValue())
    {
        long firstval = host.getFirstOperand()->getIntValue();
        long secondval = host.getSecondOperand()->getIntValue();
        
        if(secondval == 0)
    {
            printErrorMsg(&host, DIVBYZERO);
        failure = true;
        return;
    }
    
        host.setIntValue(firstval%secondval);
}
}

void TypeCheck::caseExprBitAnd(ASTExprBitAnd &host, void *param)
{
    host.getFirstOperand()->execute(*this, param);
    host.getSecondOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    if(!standardCheck(ScriptParser::TYPE_FLOAT, host.getFirstOperand()->getType(), &host)
            || !standardCheck(ScriptParser::TYPE_FLOAT, host.getSecondOperand()->getType(),&host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_FLOAT);
    
    if(host.getFirstOperand()->hasIntValue() && host.getSecondOperand()->hasIntValue())
    {
        long firstval = host.getFirstOperand()->getIntValue();
        long secondval = host.getSecondOperand()->getIntValue();
        host.setIntValue(((firstval/10000)&(secondval/10000))*10000);
    }
    }
    
void TypeCheck::caseExprBitOr(ASTExprBitOr &host, void *param)
{
    host.getFirstOperand()->execute(*this, param);
    host.getSecondOperand()->execute(*this, param);
    
    if(failure)
        return;
        
    if(!standardCheck(ScriptParser::TYPE_FLOAT, host.getFirstOperand()->getType(), &host)
            || !standardCheck(ScriptParser::TYPE_FLOAT, host.getSecondOperand()->getType(),&host))
    {
        failure = true;
        return;
    }
    
    host.setType(ScriptParser::TYPE_FLOAT);

    if(host.getFirstOperand()->hasIntValue() && host.getSecondOperand()->hasIntValue())
    {
        long firstval = host.getFirstOperand()->getIntValue();
        long secondval = host.getSecondOperand()->getIntValue();
        host.setIntValue(((firstval/10000)|(secondval/10000))*10000);
    }
}
void TypeCheck::caseExprBitXor(ASTExprBitXor &host, void *param)
    {
    host.getFirstOperand()->execute(*this, param);
    host.getSecondOperand()->execute(*this, param);
        
    if(failure)
	    return;
            
    if(!standardCheck(ScriptParser::TYPE_FLOAT, host.getFirstOperand()->getType(), &host)
       || !standardCheck(ScriptParser::TYPE_FLOAT, host.getSecondOperand()->getType(),&host))
    {
	    failure = true;
	    return;
    }
        
    host.setType(ScriptParser::TYPE_FLOAT);
    
    if(host.getFirstOperand()->hasIntValue() && host.getSecondOperand()->hasIntValue())
    {
        long firstval = host.getFirstOperand()->getIntValue();
        long secondval = host.getSecondOperand()->getIntValue();
        host.setIntValue(((firstval/10000)^(secondval/10000))*10000);
    }
}
void TypeCheck::caseExprLShift(ASTExprLShift &host, void *param)
{
    host.getFirstOperand()->execute(*this, param);
    host.getSecondOperand()->execute(*this, param);
        
        if(failure)
            return;
            
    if(!standardCheck(ScriptParser::TYPE_FLOAT, host.getFirstOperand()->getType(), &host)
            || !standardCheck(ScriptParser::TYPE_FLOAT, host.getSecondOperand()->getType(),&host))
    {
        failure = true;
        return;
    }
    
    if(host.getSecondOperand()->hasIntValue())
    {
        if(host.getSecondOperand()->getIntValue()%10000)
        {
            printErrorMsg(&host, SHIFTNOTINT);
            host.getSecondOperand()->setIntValue(10000*(host.getSecondOperand()->getIntValue()/10000));
        }
    }
    
    host.setType(ScriptParser::TYPE_FLOAT);
    
    if(host.getFirstOperand()->hasIntValue() && host.getSecondOperand()->hasIntValue())
    {
        long firstval = host.getFirstOperand()->getIntValue();
        int secondval = host.getSecondOperand()->getIntValue();
        host.setIntValue(((firstval/10000)<<(secondval/10000))*10000);
    }
}
void TypeCheck::caseExprRShift(ASTExprRShift &host, void *param)
        {
    host.getFirstOperand()->execute(*this, param);
    host.getSecondOperand()->execute(*this, param);
            
    if(failure)
        return;
                
    if(!standardCheck(ScriptParser::TYPE_FLOAT, host.getFirstOperand()->getType(), &host)
            || !standardCheck(ScriptParser::TYPE_FLOAT, host.getSecondOperand()->getType(),&host))
    {
        failure = true;
        return;
    }
            
    if(host.getSecondOperand()->hasIntValue())
            {
        if(host.getSecondOperand()->getIntValue()%10000)
                {
            printErrorMsg(&host, SHIFTNOTINT);
            host.getSecondOperand()->setIntValue(10000*(host.getSecondOperand()->getIntValue()/10000));
                }
            }
            
    host.setType(ScriptParser::TYPE_FLOAT);
    
    if(host.getFirstOperand()->hasIntValue() && host.getSecondOperand()->hasIntValue())
            {
        long firstval = host.getFirstOperand()->getIntValue();
        int secondval = host.getSecondOperand()->getIntValue();
        host.setIntValue(((firstval/10000)>>(secondval/10000))*10000);
            }
        }
        
// Other
        
bool TypeCheck::standardCheck(int firsttype, int secondtype, AST *toblame)
{
	switch(firsttype)
	{
	case ScriptParser::TYPE_BOOL:
	{
		switch(secondtype)
		{
		case ScriptParser::TYPE_BOOL:
			return true;
            
		case ScriptParser::TYPE_FLOAT:
			//Seeing as we're using float as int, this fits better with C
			/*{
			  if(toblame)
			  printErrorMsg(toblame, IMPLICITCAST, "float to bool");
			  }*/
			return true;

		case ScriptParser::TYPE_UNTYPED: return true;
            
		default:
		{
			if(toblame)
				printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to bool");
                
			return false;
		}
		}
	}
        
	case ScriptParser::TYPE_FLOAT:
	{
		switch(secondtype)
		{
		case ScriptParser::TYPE_BOOL:
		{
			if(toblame)
				printErrorMsg(toblame, ILLEGALCAST, "bool to float");
        
			return false;
		}
            
		case ScriptParser::TYPE_FLOAT:
			return true;
            
		case ScriptParser::TYPE_UNTYPED: return true;
            
		default:
		{
			if(toblame)
				printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to float");
                
			return false;
		}
		}
	}
        
	case ScriptParser::TYPE_VOID:
	{
		if(toblame)
			printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to void");
            
		return false;
	}
            
	case ScriptParser::TYPE_FFC:
	{
		if (secondtype == ScriptParser::TYPE_FFC
		    || secondtype == ScriptParser::TYPE_UNTYPED)
			return true;
            
		if(toblame)
			printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to ffc");
            
		return false;
	}
            
	case ScriptParser::TYPE_LINK:
	{
		if (secondtype == ScriptParser::TYPE_LINK
		    || secondtype == ScriptParser::TYPE_UNTYPED)
			return true;
            
		if(toblame)
			printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to link");
            
		return false;
	}
            
	case ScriptParser::TYPE_SCREEN:
	{
		if (secondtype == ScriptParser::TYPE_SCREEN
		    || secondtype == ScriptParser::TYPE_UNTYPED)
			return true;
	
		if(toblame)
			printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to screen");
            
		return false;
	}
        
	case ScriptParser::TYPE_GAME:
	{
		if (secondtype == ScriptParser::TYPE_GAME
		    || secondtype == ScriptParser::TYPE_UNTYPED)
			return true;
            
		if(toblame)
			printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to game");
            
		return false;
	}
        
	case ScriptParser::TYPE_ITEM:
	{
		if (secondtype == ScriptParser::TYPE_ITEM
		    || secondtype == ScriptParser::TYPE_UNTYPED)
			return true;
            
		if(toblame)
			printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to item");
            
		return false;
	}
        
	case ScriptParser::TYPE_ITEMCLASS:
	{
		if (secondtype == ScriptParser::TYPE_ITEMCLASS
		    || secondtype == ScriptParser::TYPE_UNTYPED)
			return true;
        
		if(toblame)
			printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to itemdata");

		return false;
	}

	case ScriptParser::TYPE_NPC:
	{
		if (secondtype == ScriptParser::TYPE_NPC
		    || secondtype == ScriptParser::TYPE_UNTYPED)
			return true;
    
		if(toblame)
			printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to npc");
            
		return false;
	}

	case ScriptParser::TYPE_LWPN:
	{
		if (secondtype == ScriptParser::TYPE_LWPN
		    || secondtype == ScriptParser::TYPE_UNTYPED)
			return true;
    
		if(toblame)
			printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to lweapon");
        
		return false;
	}
    
	case ScriptParser::TYPE_EWPN:
	{
		if (secondtype == ScriptParser::TYPE_EWPN
		    || secondtype == ScriptParser::TYPE_UNTYPED)
			return true;
        
		if(toblame)
			printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to eweapon");
    
		return false;
	}
    
	case ScriptParser::TYPE_NPCDATA:
	{
		if (secondtype == ScriptParser::TYPE_NPCDATA
		    || secondtype == ScriptParser::TYPE_UNTYPED)
			return true;
            
		if(toblame)
			printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to NPCData");
            
		return false;
	}
	case ScriptParser::TYPE_MAPDATA:
	{
		if (secondtype == ScriptParser::TYPE_MAPDATA
		    || secondtype == ScriptParser::TYPE_UNTYPED)
			return true;
            
		if(toblame)
			printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to MapData");
            
		return false;
	}
    
	case ScriptParser::TYPE_DEBUG:
	{
		if (secondtype == ScriptParser::TYPE_DEBUG
		    || secondtype == ScriptParser::TYPE_UNTYPED)
			return true;
            
		if(toblame)
			printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to Debug");
            
		return false;
	}

	case ScriptParser::TYPE_AUDIO:
	{
		if (secondtype == ScriptParser::TYPE_AUDIO
		    || secondtype == ScriptParser::TYPE_UNTYPED)
			return true;
            
		if(toblame)
			printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to Audio");
            
		return false;
	}

	case ScriptParser::TYPE_COMBOS:
	{
		if (secondtype == ScriptParser::TYPE_COMBOS
		    || secondtype == ScriptParser::TYPE_UNTYPED)
			return true;
            
		if(toblame)
			printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to ComboData");
            
		return false;
	}

	case ScriptParser::TYPE_SPRITEDATA:
	{
		if (secondtype == ScriptParser::TYPE_SPRITEDATA
		    || secondtype == ScriptParser::TYPE_UNTYPED)
			return true;
            
		if(toblame)
			printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to SpriteData");
            
		return false;
	}

	case ScriptParser::TYPE_GRAPHICS:
	{
		if (secondtype == ScriptParser::TYPE_GRAPHICS
		    || secondtype == ScriptParser::TYPE_UNTYPED)
			return true;
            
		if(toblame)
			printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to Graphics");
            
		return false;
	}

	case ScriptParser::TYPE_TEXT:
	{
		if (secondtype == ScriptParser::TYPE_TEXT
		    || secondtype == ScriptParser::TYPE_UNTYPED)
			return true;
            
		if(toblame)
			printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to Text");
            
		return false;
	}
    
	case ScriptParser::TYPE_INPUT:
	{
		if (secondtype == ScriptParser::TYPE_INPUT
		    || secondtype == ScriptParser::TYPE_UNTYPED)
			return true;
            
		if(toblame)
			printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to Input");
            
		return false;
	}

    case ScriptParser::TYPE_DMAPDATA:
    {
        if (secondtype == ScriptParser::TYPE_DMAPDATA
            || secondtype == ScriptParser::TYPE_UNTYPED)
            return true;
            
        if(toblame)
            printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to DMapData");
            
        return false;
    }
    
    case ScriptParser::TYPE_ZMESSAGE:
    {
        if (secondtype == ScriptParser::TYPE_ZMESSAGE
            || secondtype == ScriptParser::TYPE_UNTYPED)
            return true;
            
        if(toblame)
            printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to MessageData");
            
        return false;
    }

	case ScriptParser::TYPE_SHOPDATA:
        {
        if (secondtype == ScriptParser::TYPE_SHOPDATA
            || secondtype == ScriptParser::TYPE_UNTYPED )
            return true;
            
        if(toblame)
            printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to ShopData");
            
        return false;
    }

	case ScriptParser::TYPE_DROPSET:
	{
		if (secondtype == ScriptParser::TYPE_DROPSET
		    || secondtype == ScriptParser::TYPE_UNTYPED )
		    return true;
		    
		if(toblame)
		    printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to dropdata");
		    
		return false;
	}

	case ScriptParser::TYPE_PONDS:
	{
		if (secondtype == ScriptParser::TYPE_PONDS
		    || secondtype == ScriptParser::TYPE_UNTYPED )
		    return true;
		    
		if(toblame)
		    printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to ponddata");
		    
		return false;
	}

	case ScriptParser::TYPE_WARPRING:
	{
		if (secondtype == ScriptParser::TYPE_WARPRING
		    || secondtype == ScriptParser::TYPE_UNTYPED )
		    return true;
		    
		if(toblame)
		    printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to warpring");
		    
		return false;
	}

	case ScriptParser::TYPE_DOORSET:
	{
		if (secondtype == ScriptParser::TYPE_DOORSET
		    || secondtype == ScriptParser::TYPE_UNTYPED )
		    return true;
		    
		if(toblame)
		    printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to doorset");
		    
		return false;
	}

	case ScriptParser::TYPE_ZUICOLOURS:
	{
		if (secondtype == ScriptParser::TYPE_ZUICOLOURS
		    || secondtype == ScriptParser::TYPE_UNTYPED )
		    return true;
		    
		if(toblame)
		    printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to misccolors");
		    
		return false;
	}

	case ScriptParser::TYPE_RGBDATA:
	{
		if (secondtype == ScriptParser::TYPE_RGBDATA
		    || secondtype == ScriptParser::TYPE_UNTYPED )
		    return true;
		    
		if(toblame)
		    printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to rgbdata");
		    
		return false;
	}

	case ScriptParser::TYPE_PALETTE:
	{
		if (secondtype == ScriptParser::TYPE_PALETTE
		    || secondtype == ScriptParser::TYPE_UNTYPED )
		    return true;
		    
		if(toblame)
		    printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to palette");
		    
		return false;
	}

	case ScriptParser::TYPE_TUNES:
	{
		if (secondtype == ScriptParser::TYPE_TUNES
		    || secondtype == ScriptParser::TYPE_UNTYPED )
		    return true;
		    
		if(toblame)
		    printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to musictrack");
		    
		return false;
	}

	case ScriptParser::TYPE_PALCYCLE:
	{
		if (secondtype == ScriptParser::TYPE_PALCYCLE
		    || secondtype == ScriptParser::TYPE_UNTYPED )
		    return true;
		    
		if(toblame)
		    printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to palcycle");
		    
		return false;
	}

	case ScriptParser::TYPE_GAMEDATA:
	{
		if (secondtype == ScriptParser::TYPE_GAMEDATA
		    || secondtype == ScriptParser::TYPE_UNTYPED )
		    return true;
		    
		if(toblame)
		    printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to gamedata");
		    
		return false;
	}

	case ScriptParser::TYPE_CHEATS:
	{
		if (secondtype == ScriptParser::TYPE_CHEATS
		    || secondtype == ScriptParser::TYPE_UNTYPED )
		    return true;
		    
		if(toblame)
		    printErrorMsg(toblame, ILLEGALCAST, ScriptParser::printType(secondtype) + " to cheatdata");
		    
		return false;
	}
	case ScriptParser::TYPE_UNTYPED: return true;
	
    default:
        assert(false);
    }
    
    return false;
}

////////////////////////////////////////////////////////////////
// GetLValType
    
void GetLValType::caseVarDecl(ASTVarDecl &host, void *param)
    {
    GLVT *p = (GLVT *)param;
    host.execute(*(p->first.first), p->first.second);
    *(p->second) = p->first.second->first->getVarType(&host);
    }

void GetLValType::caseDefault(void *param)
{
    //these are here to bypass compiler warnings about unused arguments
    param=param;
    
    assert(false);
}

void GetLValType::caseExprArrow(ASTExprArrow &host, void *param)
{
    GLVT *p = (GLVT *)param;
    //sigh
    //get the l-hand type
    host.getLVal()->execute(*p->first.first, p->first.second);
    
    if(!p->first.first->isOK())
        return;
        
    int type = host.getLVal()->getType();
    bool isIndexed = (host.getIndex()!=0);
    
    if(isIndexed)
    {
        host.getIndex()->execute(*p->first.first, p->first.second);
        
        if(!p->first.first->isOK())
            return;
            
        if(!p->first.first->standardCheck(ScriptParser::TYPE_FLOAT, host.getIndex()->getType(),host.getIndex()))
        {
            p->first.first->fail();
            return;
        }
    }
    
    string name = "set" + host.getName();
    
    if(isIndexed)
        name += "[]";
        
    pair<int, vector<int> > fidparam;
    
    switch(type)
    {
    case ScriptParser::TYPE_FFC:
        fidparam = FFCSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_LINK:
        fidparam = LinkSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_SCREEN:
        fidparam = ScreenSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_GAME:
        fidparam = GameSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_ITEM:
        fidparam = ItemSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_ITEMCLASS:
        fidparam = ItemclassSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_NPC:
        fidparam = NPCSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_LWPN:
        fidparam = LinkWeaponSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_EWPN:
        fidparam = EnemyWeaponSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_NPCDATA:
        fidparam = NPCDataSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_MAPDATA:
        fidparam = MapDataSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_DEBUG:
        fidparam = DebugSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_AUDIO:
        fidparam = AudioSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_COMBOS:
        fidparam = CombosPtrSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_SPRITEDATA:
        fidparam = SpriteDataSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_GRAPHICS:
        fidparam = GfxPtrSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_TEXT:
        fidparam = TextPtrSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_INPUT:
        fidparam = InputSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_DMAPDATA:
        fidparam = DMapDataSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_ZMESSAGE:
        fidparam = MessageDataSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_SHOPDATA:
        fidparam = ShopDataSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_UNTYPED:
        fidparam = UntypedSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_DROPSET:
        fidparam = DropsetSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_PONDS:
        fidparam = PondSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_WARPRING:
        fidparam = WarpringSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_DOORSET:
        fidparam = DoorsetSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_ZUICOLOURS:
        fidparam = MiscColourSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_RGBDATA:
        fidparam = RGBSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_PALETTE:
        fidparam = PaletteSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_TUNES:
        fidparam = TunesSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_PALCYCLE:
        fidparam = PalCycleSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_GAMEDATA:
        fidparam = GamedataSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    case ScriptParser::TYPE_CHEATS:
        fidparam = CheatsSymbols::getInst().matchFunction(name, p->first.second->first);
        break;
    
    default:
        p->first.first->fail();
        printErrorMsg(&host, ARROWNOTPOINTER);
        return;
    }
    
    if(fidparam.first == -1 || (int)fidparam.second.size() != (isIndexed ? 3 : 2) || fidparam.second[0] != type)
    {
        printErrorMsg(&host, ARROWNOVAR, host.getName() + (isIndexed ? "[]" : ""));
        p->first.first->fail();
        return;
    }
    
    p->first.second->first->putAST(&host, fidparam.first);
    *(p->second) = fidparam.second[(isIndexed ? 2 : 1)];
}

void GetLValType::caseExprDot(ASTExprDot &host, void *param)
    {
    GLVT *p = (GLVT *)param;
    host.execute(*(p->first.first), p->first.second);
    int vid = p->first.second->first->getID(&host);
            
    if(vid == -1)
        {
        printErrorMsg(&host, LVALCONST, host.getName());
        p->first.first->fail();
            return;
        }
    
    *(p->second) = p->first.second->first->getVarType(&host);
}

void GetLValType::caseExprArray(ASTExprArray &host, void *param)
{
    GLVT *p = (GLVT *)param;
    host.execute(*(p->first.first), p->first.second);
    int vid = p->first.second->first->getID(&host);
    
    if(vid == -1)
    {
        printErrorMsg(&host, LVALCONST, host.getName());
        p->first.first->fail();
        return;
    }

    *(p->second) = p->first.second->first->getVarType(&host);
    
    if(host.getIndex())
{
        host.getIndex()->execute(*p->first.first, p->first.second);
    
        if(!p->first.first->isOK())
        return;
        
        if(!p->first.first->standardCheck(ScriptParser::TYPE_FLOAT, host.getIndex()->getType(),host.getIndex()))
    {
            p->first.first->fail();
        return;
    }
}
}

    

    
        
    

