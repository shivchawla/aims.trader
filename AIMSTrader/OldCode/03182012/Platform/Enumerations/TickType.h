#ifndef TICKTYPE_H
#define TICKTYPE_H

enum TickType { BID_SIZE, BID, ASK, ASK_SIZE, LAST, LAST_SIZE,
                HIGH, LOW, VOLUME, CLOSE,
                BID_OPTION_COMPUTATION,
                ASK_OPTION_COMPUTATION,
                LAST_OPTION_COMPUTATION,
                MODEL_OPTION,
                OPEN,
                LOW_13_WEEK,
                HIGH_13_WEEK,
                LOW_26_WEEK,
                HIGH_26_WEEK,
                LOW_52_WEEK,
                HIGH_52_WEEK,
                AVG_VOLUME,
                OPEN_INTEREST,
                OPTION_HISTORICAL_VOL,
                OPTION_IMPLIED_VOL,
                OPTION_BID_EXCH,
                OPTION_ASK_EXCH,
                OPTION_CALL_OPEN_INTEREST,
                OPTION_PUT_OPEN_INTEREST,
                OPTION_CALL_VOLUME,
                OPTION_PUT_VOLUME,
                INDEX_FUTURE_PREMIUM,
                BID_EXCH,
                ASK_EXCH,
                AUCTION_VOLUME,
                AUCTION_PRICE,
                AUCTION_IMBALANCE,
                MARK_PRICE,
                BID_EFP_COMPUTATION,
                ASK_EFP_COMPUTATION,
                LAST_EFP_COMPUTATION,
                OPEN_EFP_COMPUTATION,
                HIGH_EFP_COMPUTATION,
                LOW_EFP_COMPUTATION,
                CLOSE_EFP_COMPUTATION,
                LAST_TIMESTAMP,
                SHORTABLE,
                FUNDAMENTAL_RATIOS,
                RT_VOLUME,
                HALTED,
                BID_YIELD,
                ASK_YIELD,
                LAST_YIELD,
                CUST_OPTION_COMPUTATION,
                NOT_SET };

inline bool isPrice( TickType tickType) {
    return tickType == BID || tickType == ASK || tickType == LAST;
}

#endif // TICKTYPE_H
