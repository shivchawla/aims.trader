#include <QDebug>
#include "API/Helper.h"
#include "InboundService.h"
#include "API/Requestor.h"
#include "API/Session.h"
#include "DataAccess/ConfigurationDb.h"
#include "DataAccess/InstrumentDb.h"
#include "Utils/Constants.h"
#include <QTimer>
//using namespace std;

InboundService::InboundService(QObject *parent) : QObject(parent)
{
    //connect(this, SIGNAL(SignalInvocation()), this, SLOT(StartInbound()));
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(StartInbound()));
    timer->setSingleShot(true);
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
    QList<InstrumentData*> instruments = db.getInstruments();

    QString format = QString("yyyyMMddhhmmss");
    QDateTime start = QDateTime :: fromString(historyStartDateConf->value, "dd-MMM-yyyy");
    QDateTime end = QDateTime(QDate::currentDate(), QTime(23, 59, 59));

    if (start == QDateTime()) {
        qDebug() << "Invalid start date!! Stopping Daily History Bar Inbound." << endl;
        delete historyStartDateConf;
        return;
    }

    if (IsIgnoreCase(start, end)) {
        qDebug() << "Ignoring dates as dates culd have Sat/Sun. Start:" << start << " End: " << end << endl;
        delete historyStartDateConf;
        return;
    }

    ATTIME atBeginTime = Helper::StringToATTime(start.toString(format).toStdString());
    ATTIME atEndTime = Helper::StringToATTime(end.toString(format).toStdString());

    for(int i=0; i< instruments.count(); i++) {
        //Convert date time strings to YYYYMMDDHHMMSS format
        string sym = (instruments.at(i)->symbol).toStdString();
        qDebug() << "Fetching data for " << instruments.at(i)->symbol << " at " << QDateTime ::currentDateTime() << endl;

        ATSYMBOL atSymbol = Helper::StringToSymbol(sym);

        uint64_t requestId = requestorp->SendATBarHistoryDbRequest(atSymbol, BarHistoryDaily,
            0, atBeginTime, atEndTime, DEFAULT_REQUEST_TIMEOUT);

        printf("SEND (%llu): %s bar history request [%s]\n", requestId, "Daily", sym.c_str());

        UtilBox::sleep(3);
    }

    //Wait for a few more seconds and then update the configuration with today's date
    UtilBox::sleep(3);

    qDebug() << "All instruments data sent to server..." << endl;
    historyStartDateConf->value = QDateTime::currentDateTime().toString("dd-MMM-yyyy");
    confDb.UpdateConfiguration(historyStartDateConf, QString("HistoryStartDate"));
    qDebug() << "HistoryStartDate updated as " << historyStartDateConf->value << endl;

    //clean up memory
    qDebug() << "Cleaning up memory!" << endl;
    for(int i=0; i< instruments.count(); i++) {
        delete instruments.at(i);
    }

    //Schedule next inbound
    ScheduleNextRun();

}

void InboundService :: ScheduleNextRun() {
    //get schedule everytime because it could have changed
    ConfigurationDb confDb;
    ConfigurationData* scheduleRunTime = confDb.GetConfigurationByKey(CONF_SCHEDULE_RUNTIME);

    QTime scheduleTime = QTime::fromString(scheduleRunTime->value, "HH:mm:ss");
    if (!scheduleTime.isValid()) {
        qDebug() << "Invalid schedule time value detected in configiration. Must be in HH:mm:ss (military time) format. Defaulting to 7am" << endl;
        scheduleTime = QTime(7, 0,0); //7am
    }

    //Find how many seconds between right now and next occurance of 7am
    QDateTime now = QDateTime::currentDateTime();
    QDateTime next = now.addDays(1);
    next.setTime(scheduleTime);
    uint seconds = now.secsTo(next);

    timer->setInterval(seconds*1000);
    timer->start();
    qDebug() << "Inbound scheduled to run next time at " << next << endl;
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

//Uses start date and end date to determine if dates should be ignored and data not fetched from server
// as the dates might signify saturday and sunday
bool InboundService :: IsIgnoreCase(QDateTime startDate, QDateTime endDate) {
    QString startDay = startDate.toString("ddd");
    QString endDay = endDate.toString("ddd");
    if ((startDay == "Sat" && endDay == "Sun") ||
        (startDate.date() == endDate.date() && (startDay == "Sat" || startDay == "Sun")))
        return true; //means this is a ignore case
    else
        return false;
}
