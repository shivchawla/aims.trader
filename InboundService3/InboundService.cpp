#include <QDebug>
#include "Helper.h"
#include "InboundService.h"
#include "Requestor.h"
#include "Session.h"
#include "ConfigurationDb.h"
#include "InstrumentDb.h"
//using namespace std;

InboundService::InboundService()// : QObject(parent)
{
    //connect(this, SIGNAL(SignalInvocation()), this, SLOT(StartInbound()));
}

InboundService::~InboundService()
{
}

void InboundService::Init() {
	qDebug() << "Starting service ..." << endl;
	ATInitAPI();

	/*APISession session;
		Requestor requestor(session); */

    sessionp = new APISession(/*this*/);
    requestorp = new Requestor(*sessionp);

	//login now
	string serverIpAddress, apiUserid, userid, password;

	serverIpAddress="activetick1.activetick.com";
	apiUserid="4ca0f31fbc8df528b598dfd41368af3f";
	userid="shivchawla";
	password= "27as04sh";

	Logon(serverIpAddress, apiUserid, userid, password);
    sessionp->WaitForSession();

    StartInbound();
}

void InboundService :: Logon(string serverAddress, string apiUserId, string userId, string password) {
	uint32_t serverPort = 0;
				
    ATGUID guidApiUserid = Helper::StringToATGuid(apiUserId);
	bool rc = sessionp->Init(guidApiUserid, serverAddress, serverPort, &Helper::ConvertString(userId).front(), &Helper::ConvertString(password).front());
	printf("init status: %d\n", rc);
    sessionp->WaitForSession();
}

void InboundService :: StartInbound() {
	qDebug() << "Starting inbound..." << endl;
    //return;

    ConfigurationDb confDb;
    ConfigurationData* historyStartDateConf = confDb.GetConfigurationByKey("HistoryStartDate");

    InstrumentDb db;
    QList<InstrumentData*> instruments = db.GetInstruments();

    for(int i=0; i< instruments.count(); i++) {
        qDebug() << "Fetching data for " << instruments.at(i)->symbol << " at " << QDateTime ::currentDateTime() << endl;

        QString format = QString("yyyyMMddhhmmss");
        //Convert date time strings to YYYYMMDDHHMMSS format
        string sym = (instruments.at(i)->symbol).toStdString();

        QDateTime start = QDateTime :: fromString(historyStartDateConf->value, "dd-MMM-yyyy");
        QDateTime end = QDateTime(QDate::currentDate(), QTime(23, 59, 59));

        ATTIME atBeginTime = Helper::StringToATTime(start.toString(format).toStdString());
        ATTIME atEndTime = Helper::StringToATTime(end.toString(format).toStdString());


        ATSYMBOL atSymbol = Helper::StringToSymbol(sym);

        uint64_t requestId = requestorp->SendATBarHistoryDbRequest(atSymbol, BarHistoryDaily,
            0, atBeginTime, atEndTime, DEFAULT_REQUEST_TIMEOUT);

        printf("SEND (%llu): %s bar history request [%s]\n", requestId, "Daily", sym.c_str());

        UtilBox::sleep(3);
    }

    //Wait for a few more seconds and then update the configuration with today's date
    UtilBox::sleep(3);

    qDebug() << "All instruments data sent to server..." << endl;
    historyStartDateConf->value = QDateTime::currentDateTime().toString("MM/dd/yyyy");
    confDb.UpdateConfiguration(historyStartDateConf, QString("HistoryStartDate"));
    qDebug() << "HistoryStartDate updated as " << historyStartDateConf->value << endl;

}

void InboundService :: Shutdown() {
	qDebug() << "Stopping service..." << endl;
	delete requestorp;
	sessionp->Destroy();
	ATShutdownAPI();
	delete sessionp;
}

void InboundService :: InvokeService() {
	qDebug() << "Invoking service" << endl;
    //emit SignalInvocation();
}

