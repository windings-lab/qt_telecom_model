#pragma once

#include "models.h"


class IDataRepository {
public:
	virtual ~IDataRepository() = default;
	virtual QList<Country> loadCountries() = 0;
};
