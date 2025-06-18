#pragma once

struct Operator {
	int mcc;
	int mnc;
	QString name;
};

struct CountryData {
	int mcc;
	int mcc_length;
	QList<Operator> operators;
};

struct Country {
	QString code;
	QString name;
	QList<CountryData> data;
};