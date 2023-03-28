// Not copyrighted - public domain.
//
// This sample parser implementation was generated by CodeSynthesis XSD,
// an XML Schema to C++ data binding compiler. You may use it in your
// programs without any restrictions.
//

#include <iostream>
#include <sstream>
#include <string>

#include <include/exception/dnaexception.hpp>
#include <include/parameters/dnaepsg.hpp>
#include <dynadjust/dnaimport/dnaparser_pimpl.hxx>
#include <include/measurement_types/dnastntally.hpp>

#include <include/io/DynaML-schema.hxx>

using namespace std;
using namespace dynadjust::measurements;
using namespace dynadjust::epsg;
using namespace dynadjust::exception;

extern MsrTally g_parsemsr_tally;
extern StnTally g_parsestn_tally;
extern UINT32 g_fileOrder;

// Clusterpoint_pimpl
//

void Clusterpoint_pimpl::pre()
{
	_dnaGpsPoint.reset(new CDnaGpsPoint);
	// The following is needed so that SetX and SetY can properly handle
	// latitude|X and longitude|Y
	_dnaGpsPoint->SetType(_parent_dnaGpsPointCluster->GetType());
	_dnaGpsPoint->SetCoordType(_parent_dnaGpsPointCluster->GetCoordType());
	_dnaGpsPoint->SetFirst(_parent_dnaGpsPointCluster->GetFirst());
	_dnaGpsPoint->SetClusterID(_parent_dnaGpsPointCluster->GetClusterID());
	
	_dnaGpsPoint->SetReferenceFrame(_parent_dnaGpsPointCluster->GetReferenceFrame());
	_dnaGpsPoint->SetEpoch(_parent_dnaGpsPointCluster->GetEpoch()); 
		
	_dnaGpsPoint->SetClusterDBID(
		_parent_dnaGpsPointCluster->GetClusterDBID(),
		_parent_dnaGpsPointCluster->GetClusterDBIDset());

}

void Clusterpoint_pimpl::X (const ::std::string& X)
{
	_dnaGpsPoint->SetX(X);

	// increment measurement counter
	*(_pMeasurementCount) += 1;
	g_parsemsr_tally.Y++;
}

void Clusterpoint_pimpl::Y (const ::std::string& Y)
{
	_dnaGpsPoint->SetY(Y);

	// increment measurement counter
	*(_pMeasurementCount) += 1;
	g_parsemsr_tally.Y++;
}

void Clusterpoint_pimpl::Z (const ::std::string& Z)
{
	_dnaGpsPoint->SetZ(Z);

	// increment measurement counter
	*(_pMeasurementCount) += 1;
	g_parsemsr_tally.Y++;
}

void Clusterpoint_pimpl::MeasurementDBID(const ::std::string& MeasurementID)
{
	_dnaGpsPoint->SetMeasurementDBID(MeasurementID);
	_dnaGpsPoint->SetClusterDBID(
		_parent_dnaGpsPointCluster->GetClusterDBID(),
		_parent_dnaGpsPointCluster->GetClusterDBIDset());
}


void Clusterpoint_pimpl::SigmaXX (const ::std::string& SigmaXX)
{
	_dnaGpsPoint->SetSigmaXX(SigmaXX);
}

void Clusterpoint_pimpl::SigmaXY (const ::std::string& SigmaXY)
{
	_dnaGpsPoint->SetSigmaXY(SigmaXY);
}

void Clusterpoint_pimpl::SigmaXZ (const ::std::string& SigmaXZ)
{
	_dnaGpsPoint->SetSigmaXZ(SigmaXZ);
}

void Clusterpoint_pimpl::SigmaYY(const ::std::string& SigmaYY)
{
	_dnaGpsPoint->SetSigmaYY(SigmaYY);
}

void Clusterpoint_pimpl::SigmaYZ(const ::std::string& SigmaYZ)
{
	_dnaGpsPoint->SetSigmaYZ(SigmaYZ);
}

void Clusterpoint_pimpl::SigmaZZ(const ::std::string& SigmaZZ)
{
	_dnaGpsPoint->SetSigmaZZ(SigmaZZ);
}

void Clusterpoint_pimpl::PointCovariance ()
{
}

void Clusterpoint_pimpl::post_Clusterpoint ()
{
	if (_dnaGpsPoint && _parent_dnaGpsPointCluster)
		_parent_dnaGpsPointCluster->AddGpsPoint(_dnaGpsPoint.get());
}

// Directions_pimpl
//

void Directions_pimpl::pre()
{
	_dnaDirection.reset(new CDnaDirection);
	_dnaDirection->SetClusterID(_parent_dnaDirectionSet->GetClusterID());
}

void Directions_pimpl::Ignore(const ::std::string& Ignore)
{
	if (Ignore.empty())
		_dnaDirection->SetIgnore(false);
	else
		_dnaDirection->SetIgnore(true);
}

void Directions_pimpl::Target(const ::std::string& Target)
{
	_dnaDirection->SetTarget(Target);
}

void Directions_pimpl::Value(const ::std::string& Value)
{
	_dnaDirection->SetValue(Value);

	if (_dnaDirection->NotIgnored())
	{
		// increment measurement counter
		*(_pMeasurementCount) += 1;
		g_parsemsr_tally.D++;
	}
}

void Directions_pimpl::StdDev(const ::std::string& StdDev)
{
	_dnaDirection->SetStdDev(StdDev);

}
	

void Directions_pimpl::MeasurementDBID(const ::std::string& MeasurementID)
{
	_dnaDirection->SetMeasurementDBID(MeasurementID);
	_dnaDirection->SetClusterDBID(
		_parent_dnaDirectionSet->GetClusterDBID(),
		_parent_dnaDirectionSet->GetClusterDBIDset());
}
	

void Directions_pimpl::post_Directions(const UINT32& total)
{
	if (!_parent_dnaDirectionSet)
		return;

	if (_dnaDirection->NotIgnored())
		_parent_dnaDirectionSet->AddDirection(_dnaDirection.get());
	else
	{
		UINT32 t = _parent_dnaDirectionSet->GetTotal() - 1;
		_parent_dnaDirectionSet->SetTotal(t);

		if (t == 0)
		{
			// Is the entire direction set ignored?
			if (_parent_dnaDirectionSet->GetIgnore())
				return;

			stringstream ss, ss2;
			ss << 
				"<Directions>...</Directions>',  total of " << total << " element(s)" << endl <<
				"  - found 0 <Directions>, or there aren't any non-ignored directions in the set. '";
			ss2 << " ~ <Total>" << total << "</Total>";
			throw ::xsd::cxx::parser::expected_element< char >(
				ss.str(), ss2.str());
		}
	}
}

// DnaMeasurement_pimpl
//

void DnaMeasurement_pimpl::pre()
{
	_dnaCurrentMsr.reset();
}


void DnaMeasurement_pimpl::Type(const ::std::string& Type)
{
	if (Type.empty())
		throw XMLInteropException("\"Type\" element cannot be empty.", 0);

	char cType = (Type.c_str())[0];
	switch (cType) {
	case 'A': // Horizontal angle
		g_parsemsr_tally.A++;
		*(_pMeasurementCount) += 1;
		_dnaCurrentMsr.reset(new CDnaAngle);
		break;
	case 'B': // Geodetic azimuth
		g_parsemsr_tally.B++;
		*(_pMeasurementCount) += 1;
		_dnaCurrentMsr.reset(new CDnaAzimuth);
		break;
	case 'C': // Chord dist
		g_parsemsr_tally.C++;
		*(_pMeasurementCount) += 1;
		_dnaCurrentMsr.reset(new CDnaDistance);
		break;
	case 'D': // Direction set
		_dnaCurrentMsr.reset(new CDnaDirectionSet(++(*(_pclusterID))));
		break;
	case 'E': // Ellipsoid arc
		g_parsemsr_tally.E++;
		*(_pMeasurementCount) += 1;
		_dnaCurrentMsr.reset(new CDnaDistance);
		break;
	case 'G': // GPS Baseline (treat as single-baseline cluster)
	case 'X': // GPS Baseline cluster
		_dnaCurrentMsr.reset(new CDnaGpsBaselineCluster(++(*(_pclusterID)), _referenceframe, _epoch));
		break;
	case 'H': // Orthometric height
		g_parsemsr_tally.H++;
		*(_pMeasurementCount) += 1;
		_dnaCurrentMsr.reset(new CDnaHeight);
		break;
	case 'I': // Astronomic latitude
		g_parsemsr_tally.I++;
		*(_pMeasurementCount) += 1;
		_dnaCurrentMsr.reset(new CDnaCoordinate);
		break;
	case 'J': // Astronomic longitude
		g_parsemsr_tally.J++;
		*(_pMeasurementCount) += 1;
		_dnaCurrentMsr.reset(new CDnaCoordinate);
		break;
	case 'K': // Astronomic azimuth
		g_parsemsr_tally.K++;
		*(_pMeasurementCount) += 1;
		_dnaCurrentMsr.reset(new CDnaAzimuth);
		break;
	case 'L': // Level difference
		g_parsemsr_tally.L++;
		*(_pMeasurementCount) += 1;
		_dnaCurrentMsr.reset(new CDnaHeightDifference);
		break;
	case 'M': // MSL arc
		g_parsemsr_tally.M++;
		*(_pMeasurementCount) += 1;
		_dnaCurrentMsr.reset(new CDnaDistance);
		break;
	case 'P': // Geodetic latitude
		g_parsemsr_tally.P++;
		*(_pMeasurementCount) += 1;
		_dnaCurrentMsr.reset(new CDnaCoordinate);
		break;
	case 'Q': // Geodetic longitude
		g_parsemsr_tally.Q++;
		*(_pMeasurementCount) += 1;
		_dnaCurrentMsr.reset(new CDnaCoordinate);
		break;
	case 'R': // Ellipsoidal height
		g_parsemsr_tally.R++;
		*(_pMeasurementCount) += 1;
		_dnaCurrentMsr.reset(new CDnaHeight);
		break;
	case 'S': // Slope distance
		g_parsemsr_tally.S++;
		*(_pMeasurementCount) += 1;
		_dnaCurrentMsr.reset(new CDnaDistance);
		break;
	case 'V': // Zenith angle
		g_parsemsr_tally.V++;
		*(_pMeasurementCount) += 1;
		_dnaCurrentMsr.reset(new CDnaDirection);
		break;
	case 'Y': // GPS point cluster
		_dnaCurrentMsr.reset(new CDnaGpsPointCluster(++(*(_pclusterID)), _referenceframe, _epoch));
		break;
	case 'Z': // Vertical angle
		g_parsemsr_tally.Z++;
		*(_pMeasurementCount) += 1;
		_dnaCurrentMsr.reset(new CDnaDirection);
		break;
	default:
		stringstream ss;
		ss << "Unknown measurement type: " << Type;
		throw XMLInteropException(ss.str(), 0);
	}
	_dnaCurrentMsr->SetType(Type);
}

void DnaMeasurement_pimpl::Ignore(const ::std::string& Ignore)
{
	if (!_dnaCurrentMsr)
		throw XMLInteropException("\"Type\" element must be the first element within \"DnaMeasurement\".", 0);

	if (Ignore.empty())
		_dnaCurrentMsr->SetIgnore(false);
	else
	{
		_dnaCurrentMsr->SetIgnore(true);
		g_parsemsr_tally.ignored++;
	}
}

void DnaMeasurement_pimpl::First(const ::std::string& First)
{
	if (!_dnaCurrentMsr)
		throw XMLInteropException("\"Type\" element must be the first element within \"DnaMeasurement\".", 0);

	if (First.empty())
		throw XMLInteropException("\"First\" element cannot be empty.", 0);
	_dnaCurrentMsr->SetFirst(First);
}

void DnaMeasurement_pimpl::Second(const ::std::string& Second) 
{
	if (!_dnaCurrentMsr)
		throw XMLInteropException("\"Type\" element must be the first element within \"DnaMeasurement\".", 0);

	switch (_dnaCurrentMsr->GetTypeC()) {
	case 'A': // Horizontal angle
	case 'B': // Geodetic azimuth
	case 'K': // Astronomic azimuth
	case 'C': // Chord dist
	case 'E': // Ellipsoid arc
	case 'M': // MSL arc
	case 'S': // Slope distance
	case 'D': // Direction set
	case 'G': // GPS Baseline
	case 'L': // Level difference
	case 'V': // Zenith angle
	case 'Z': // Vertical angle
	case 'X': // GPS Baseline cluster
		if (Second.empty())
		{
			stringstream ss;
			ss << "\"Second\" element cannot be empty for measurement type " << _dnaCurrentMsr->GetTypeC() << 
				" (first station: " << _dnaCurrentMsr->GetFirst() << ").";
			throw XMLInteropException(ss.str(), 0);
		}
		_dnaCurrentMsr->SetTarget(Second);
	}
}

void DnaMeasurement_pimpl::Third(const ::std::string& Third)
{
	if (!_dnaCurrentMsr)
		throw XMLInteropException("\"Type\" element must be the first element within \"DnaMeasurement\".", 0);

	switch (_dnaCurrentMsr->GetTypeC()) {
	case 'A': // Horizontal angle
		if (Third.empty())
		{
			stringstream ss;
			ss << "\"Third\" element cannot be empty for measurement type " << _dnaCurrentMsr->GetTypeC() << 
				" (first station: " << _dnaCurrentMsr->GetFirst() << ").";
			throw XMLInteropException(ss.str(), 0);
		}
	}

	_dnaCurrentMsr->SetTarget2(Third);
}

void DnaMeasurement_pimpl::Value(const ::std::string& Value)
{
	if (!_dnaCurrentMsr)
		throw XMLInteropException("\"Type\" element must be the first element within \"DnaMeasurement\".", 0);

	switch (_dnaCurrentMsr->GetTypeC()) {
	case 'A': // Horizontal angle
	case 'B': // Geodetic azimuth
	case 'K': // Astronomic azimuth
	case 'C': // Chord dist
	case 'E': // Ellipsoid arc
	case 'M': // MSL arc
	case 'S': // Slope distance
	case 'D': // Direction set
	case 'H': // Orthometric height
	case 'R': // Ellipsoidal height
	case 'I': // Astronomic latitude
	case 'J': // Astronomic longitude
	case 'P': // Geodetic latitude
	case 'Q': // Geodetic longitude
	case 'L': // Level difference
	case 'V': // Zenith angle
	case 'Z': // Vertical angle
		if (Value.empty())
		{
			stringstream ss;
			ss << "\"Value\" element cannot be empty for measurement type " << _dnaCurrentMsr->GetTypeC() << 
				" (first station: " << _dnaCurrentMsr->GetFirst() << ").";
			throw XMLInteropException(ss.str(), 0);
		}
	}

	_dnaCurrentMsr->SetValue(Value);
}

void DnaMeasurement_pimpl::StdDev(const ::std::string& StdDev)
{
	if (!_dnaCurrentMsr)
		throw XMLInteropException("\"Type\" element must be the first element within \"DnaMeasurement\".", 0);

	switch (_dnaCurrentMsr->GetTypeC()) {
	case 'A': // Horizontal angle
	case 'B': // Geodetic azimuth
	case 'K': // Astronomic azimuth
	case 'C': // Chord dist
	case 'E': // Ellipsoid arc
	case 'M': // MSL arc
	case 'S': // Slope distance
	case 'D': // Direction set
	case 'H': // Orthometric height
	case 'R': // Ellipsoidal height
	case 'I': // Astronomic latitude
	case 'J': // Astronomic longitude
	case 'P': // Geodetic latitude
	case 'Q': // Geodetic longitude
	case 'L': // Level difference
	case 'V': // Zenith angle
	case 'Z': // Vertical angle
		if (StdDev.empty())
		{
			stringstream ss;
			ss << "\"StdDev\" element cannot be empty for " << _dnaCurrentMsr->GetTypeC() << " measurements (first station: " << _dnaCurrentMsr->GetFirst() << ").";
			throw XMLInteropException(ss.str(), 0);
		}

		if (DoubleFromString<double>(StdDev) < PRECISION_1E25)
		{
			stringstream ss;
			ss << "\"StdDev\" element cannot contain a zero or negative value for " << _dnaCurrentMsr->GetTypeC() << " measurements (first station: " << _dnaCurrentMsr->GetFirst() << ").";
			throw XMLInteropException(ss.str(), 0);
		}
	}

	_dnaCurrentMsr->SetStdDev(StdDev);

}

void DnaMeasurement_pimpl::InstHeight(const ::std::string& InstHeight)
{
	if (!_dnaCurrentMsr)
		throw XMLInteropException("\"Type\" element must be the first element within \"DnaMeasurement\".", 0);

	// Instrument and target heights only make sense for 
	// slope distances, vertical angles and zenith distances.
	// Don't need instrument height for all other types.
	switch (_dnaCurrentMsr->GetTypeC()) {
	case 'S':
	case 'V':
	case 'Z':
		_dnaCurrentMsr->SetInstrumentHeight(InstHeight);
		break;
	}
}

void DnaMeasurement_pimpl::TargHeight(const ::std::string& TargHeight)
{
	if (!_dnaCurrentMsr)
		throw XMLInteropException("\"Type\" element must be the first element within \"DnaMeasurement\".", 0);

	// Instrument and target heights only make sense for 
	// slope distances, vertical angles and zenith distances.
	// Don't need instrument height for all other types.
	switch (_dnaCurrentMsr->GetTypeC()) {
	case 'S':
	case 'V':
	case 'Z':
		_dnaCurrentMsr->SetTargetHeight(TargHeight);
		break;
	}
}

void DnaMeasurement_pimpl::Total(const ::std::string& Total)
{
	if (!_dnaCurrentMsr)
		throw XMLInteropException("\"Type\" element must be the first element within \"DnaMeasurement\".", 0);

	switch (_dnaCurrentMsr->GetTypeC()) {
	case 'D': // Direction set
	case 'G': // Single Baseline (treat as a single-baseline cluster)
	case 'X': // GPS Baseline cluster
	case 'Y': // GPS point cluster
		if (Total.empty())
			throw XMLInteropException("\"Total\" element cannot be empty.", 0);
	}

	_dnaCurrentMsr->SetTotal(Total);

	const UINT32 measCount(LongFromString<UINT32>(Total));
	_total = measCount;

	switch (_dnaCurrentMsr->GetTypeC()) 
	{
	case 'X': // GPS Baseline cluster
		if (measCount < 2)
			if (_preferSingleXasG)
				_dnaCurrentMsr->SetType("G");
	}
}

void DnaMeasurement_pimpl::Directions()
{
}

void DnaMeasurement_pimpl::Vscale(const ::std::string& Vscale)
{
	if (!_dnaCurrentMsr)
		throw XMLInteropException("\"Type\" element must be the first element within \"DnaMeasurement\".", 0);

	if (Vscale.empty())
		_dnaCurrentMsr->SetVscale("1");
	else
		_dnaCurrentMsr->SetVscale(Vscale);
}

void DnaMeasurement_pimpl::Epoch(const ::std::string& Epoch)
{
	if (!_dnaCurrentMsr)
		throw XMLInteropException("\"Type\" element must be the first element within \"DnaMeasurement\".", 0);

	try
	{
		// Override the reference frame with the user specified frame?
		if (_overridereferenceframe)
		{
			_dnaCurrentMsr->SetEpoch(_epoch);
			return;
		}

		if (Epoch.empty())
			// Do nothing - use default epoch
			return;

		_dnaCurrentMsr->SetEpoch(Epoch);
	}
	catch (const runtime_error& e) {
		stringstream ss("");
		ss << e.what();
		ss << "    - Measurement type: " << _dnaCurrentMsr->GetType() << endl <<
			"    - From:             " << _dnaCurrentMsr->GetFirst() << endl <<
			"    - To:               " << _dnaCurrentMsr->GetTarget() << endl <<
			"    - Reference frame:  " << _dnaCurrentMsr->GetReferenceFrame() << endl <<
			"    - Epoch:            " << Epoch << endl;
		throw XMLInteropException(ss.str(), 0);
	}
}

void DnaMeasurement_pimpl::ReferenceFrame(const ::std::string& ReferenceFrame)
{
	if (!_dnaCurrentMsr)
		throw XMLInteropException("\"Type\" element must be the first element within \"DnaMeasurement\".", 0);

	try
	{
		// Set the reference frame found for this measurement
		_dnaCurrentMsr->SetReferenceFrame(ReferenceFrame);

		// Override the reference frame with the user specified frame?
		if (_overridereferenceframe)
		{
			_dnaCurrentMsr->SetReferenceFrame(_referenceframe);
			return;
		}

		// Was no frame supplied?  If so, do nothing (use default reference frame)
		if (ReferenceFrame.empty())
			return;

		// Set the reference frame found for this measurement
		_dnaCurrentMsr->SetReferenceFrame(ReferenceFrame);
	}
	catch (const runtime_error& e) {
		stringstream ss("");
		ss << e.what();
		ss << "    - Measurement type: " << _dnaCurrentMsr->GetType() << endl <<
			"    - From:             " << _dnaCurrentMsr->GetFirst() << endl <<
			"    - To:               " << _dnaCurrentMsr->GetTarget() << endl <<
			"    - Reference frame:  " << ReferenceFrame << endl <<
			"    - Epoch:            " << _dnaCurrentMsr->GetEpoch() << endl;
		throw XMLInteropException(ss.str(), 0);
	}
}

void DnaMeasurement_pimpl::GPSBaseline()
{
}


void DnaMeasurement_pimpl::Hscale(const ::std::string& Hscale)
{
	if (!_dnaCurrentMsr)
		throw XMLInteropException("\"Type\" element must be the first element within \"DnaMeasurement\".", 0);

	if (Hscale.empty())
		_dnaCurrentMsr->SetHscale("1");
	else
		_dnaCurrentMsr->SetHscale(Hscale);
}


void DnaMeasurement_pimpl::Lscale(const ::std::string& Lscale)
{
	if (!_dnaCurrentMsr)
		throw XMLInteropException("\"Type\" element must be the first element within \"DnaMeasurement\".", 0);

	if (Lscale.empty())
		_dnaCurrentMsr->SetLscale("1");
	else
		_dnaCurrentMsr->SetLscale(Lscale);
}


void DnaMeasurement_pimpl::Pscale(const ::std::string& Pscale)
{
	if (!_dnaCurrentMsr)
		throw XMLInteropException("\"Type\" element must be the first element within \"DnaMeasurement\".", 0);

	if (Pscale.empty())
		_dnaCurrentMsr->SetPscale("1");
	else
		_dnaCurrentMsr->SetPscale(Pscale);
}


void DnaMeasurement_pimpl::Clusterpoint()
{
}

void DnaMeasurement_pimpl::Coords(const ::std::string& Coords)
{
	if (!_dnaCurrentMsr)
		throw XMLInteropException("\"Type\" element must be the first element within \"DnaMeasurement\".", 0);

	_dnaCurrentMsr->SetCoordType(Coords);
}


void DnaMeasurement_pimpl::Source(const ::std::string& Source)
{
	if (!_dnaCurrentMsr)
		throw XMLInteropException("\"Type\" element must be the first element within \"DnaMeasurement\".", 0);

	if (Source.empty())
		_dnaCurrentMsr->SetSource("");
	else
		_dnaCurrentMsr->SetSource(Source);
}
	

void DnaMeasurement_pimpl::MeasurementDBID(const ::std::string& MeasurementID)
{
	if (!_dnaCurrentMsr)
		throw XMLInteropException("\"Type\" element must be the first element within \"DnaMeasurement\".", 0);

	if (MeasurementID.empty())
		_dnaCurrentMsr->SetMeasurementDBID("");
	else
		_dnaCurrentMsr->SetMeasurementDBID(MeasurementID);
}
	

void DnaMeasurement_pimpl::ClusterDBID(const ::std::string& ClusterID)
{
	if (!_dnaCurrentMsr)
		throw XMLInteropException("\"Type\" element must be the first element within \"DnaMeasurement\".", 0);

	if (ClusterID.empty())
		_dnaCurrentMsr->SetClusterDBID("");
	else
		_dnaCurrentMsr->SetClusterDBID(ClusterID);
}
	

void DnaMeasurement_pimpl::post_DnaMeasurement()
{
	if (!_dnaCurrentMsr)
		throw XMLInteropException("\"Type\" element must be the first element within \"DnaMeasurement\".", 0);

	UINT32 total, found;

	// Is the direction set empty and ignored?
	// If so, do nothing.
	switch (_dnaCurrentMsr->GetTypeC())
	{
	case 'D':
		total = _dnaCurrentMsr->GetTotal();
		if (total == 0 && _dnaCurrentMsr->GetIgnore())
			return;
		break;
	}

	// Now that the measurement has completed, capture all essential elements
	_vParentMsrs->push_back(_dnaCurrentMsr);

	// set "First" to be that of the first in the set
	switch (_dnaCurrentMsr->GetTypeC())
	{
	case 'D':
	
		total = _dnaCurrentMsr->GetTotal();
		found = static_cast<UINT32>(_dnaCurrentMsr->GetDirections_ptr()->size());
		if (total != found)
		{
			stringstream ss, ss2;
			ss <<
				"<Directions>...</Directions>',  total of " << total << " element(s)" << endl <<
				"  - found " << found << " <Directions> in the set. '";
			ss2 << " ~ <Total>" << total << "</Total>";
			throw ::xsd::cxx::parser::expected_element< char >(
				ss.str(), ss2.str());
		}

		// Okay, capture all the elements for this baseline (cluster)
		for_each(_dnaCurrentMsr->GetDirections_ptr()->begin(),
			_dnaCurrentMsr->GetDirections_ptr()->end(),
			[this](CDnaDirection& d) {
				d.SetType(_dnaCurrentMsr->GetType());
				d.SetFirst(_dnaCurrentMsr->GetFirst());
				// This is a target direction, so set zero recorded total so as to
				// distinguish this direction from the RO, for which the recorded
				// total will be the number of target directions
				d.SetRecordedTotal(0);
			});

		break;
	
	case 'G':
	case 'X':
	
		total = _dnaCurrentMsr->GetTotal();
		found = static_cast<UINT32>(_dnaCurrentMsr->GetBaselines_ptr()->size());
		if (total != found)
		{
			stringstream ss, ss2;
			ss <<
				"<GPSBaseline>...</GPSBaseline>',  total of " << total << " element(s)" << endl <<
				"  - found " << found << " <GPSBaseline> in the set. '";
			ss2 << " ~ <Total>" << total << "</Total>";
			throw ::xsd::cxx::parser::expected_element< char >(
				ss.str(), ss2.str());
		}

		// Okay, capture all the elements for this baseline (cluster)
		for_each(_dnaCurrentMsr->GetBaselines_ptr()->begin(),
			_dnaCurrentMsr->GetBaselines_ptr()->end(),
			[this](CDnaGpsBaseline& b) {
				b.SetIgnore(_dnaCurrentMsr->GetIgnore());
				b.SetRecordedTotal(_dnaCurrentMsr->GetTotal());

				b.SetPscale(_dnaCurrentMsr->GetPscale());
				b.SetLscale(_dnaCurrentMsr->GetLscale());
				b.SetHscale(_dnaCurrentMsr->GetHscale());
				b.SetVscale(_dnaCurrentMsr->GetVscale());
			});

		break;
	
	case 'Y':
	
		total = _dnaCurrentMsr->GetTotal();
		found = static_cast<UINT32>(_dnaCurrentMsr->GetPoints_ptr()->size());
		if (total != found)
		{
			stringstream ss, ss2;
			ss <<
				"<Clusterpoint>...</Clusterpoint>',  total of " << total << " element(s)" << endl <<
				"  - found " << found << " <Clusterpoint> in the set. '";
			ss2 << " ~ <Total>" << total << "</Total>";
			throw ::xsd::cxx::parser::expected_element< char >(
				ss.str(), ss2.str());
		}

		// Okay, capture all the elements for this point cluster
		for_each(_dnaCurrentMsr->GetPoints_ptr()->begin(),
			_dnaCurrentMsr->GetPoints_ptr()->end(),
			[this](CDnaGpsPoint& p) {
				p.SetIgnore(_dnaCurrentMsr->GetIgnore());
				p.SetCoordType(_dnaCurrentMsr->GetCoordType());
				p.SetRecordedTotal(_dnaCurrentMsr->GetTotal());

				p.SetPscale(_dnaCurrentMsr->GetPscale());
				p.SetLscale(_dnaCurrentMsr->GetLscale());
				p.SetHscale(_dnaCurrentMsr->GetHscale());
				p.SetVscale(_dnaCurrentMsr->GetVscale());
			});
		break;
	
	}

}

// DnaStation_pimpl
//

void DnaStation_pimpl::pre()
{
	_dnaCurrentStn.reset(new CDnaStation(_referenceframe, _epoch));
	_dnaCurrentStn->SetfileOrder(g_fileOrder++);
}

void DnaStation_pimpl::Name(const ::std::string& Name)
{
	if (Name.empty())
		throw XMLInteropException("DnaStation \"Name\" element cannot be empty.", 0);
	_dnaCurrentStn->SetName(Name);
}


void DnaStation_pimpl::Constraints(const ::std::string& Constraints)
{
	if (Constraints.empty())	
		_dnaCurrentStn->SetConstraints("FFF");
	else
		_dnaCurrentStn->SetConstraints(Constraints);
	g_parsestn_tally.addstation(Constraints);
}


void DnaStation_pimpl::Type(const ::std::string& Type)
{
	_dnaCurrentStn->SetCoordType(Type);
}

void DnaStation_pimpl::StationCoord()
{
}

void DnaStation_pimpl::Description(const ::std::string& Description)
{
	_dnaCurrentStn->SetDescription(
		findandreplace((const ::std::string&)Description, (const ::std::string&)"&", (const ::std::string&)"and"));
}

void DnaStation_pimpl::post_DnaStation()
{
	_vParentStns->push_back(_dnaCurrentStn);
}

// DnaXmlFormat_pimpl
//

DnaXmlFormat_pimpl::DnaXmlFormat_pimpl(std::ifstream* is, PUINT32 clusterID, const string& referenceframe, const string& epoch, bool userspecifiedreferenceframe, bool overridereferenceframe)
{
	// capture pointer to file stream
	is_ = is;
	
	// capture global cluster ID to permit sequential numbering over several input files
	_clusterID = *clusterID;	

	// capture default reference frame and epoch
	_referenceframe = referenceframe;
	_epoch = epoch;

	// Has the user supplied a reference frame and should this be used to override all?
	_userspecifiedreferenceframe = userspecifiedreferenceframe;
	_overridereferenceframe = overridereferenceframe;
}

void DnaXmlFormat_pimpl::pre()
{
	_pStns = new vdnaStnPtr ;
	_pMsrs = new vdnaMsrPtr ;

	g_parsemsr_tally.initialise();
	g_parsestn_tally.initialise();
	_station_count = 0;
	_measurement_count = 0;
}

void DnaXmlFormat_pimpl::DnaStation()
{
	_station_count++;
}

void DnaXmlFormat_pimpl::DnaMeasurement()
{
}

void DnaXmlFormat_pimpl::type()
{
}

void DnaXmlFormat_pimpl::referenceframe()
{
}

void DnaXmlFormat_pimpl::epoch()
{
}

void DnaXmlFormat_pimpl::post_DnaXmlFormat(vdnaStnPtr* vStations, vdnaMsrPtr* vMeasurements) 
{
	ostringstream ss("");
	if (_station_count > 0) 
	{
		ss << "Loaded " << _station_count << " stations";
		_parse_msg = ss.str();
		(*vStations) = (vdnaStnPtr) (*(_pStns));
	}
	if (_measurement_count > 0) 
	{
		ss << "Loaded " << _measurement_count << " measurements";
		_parse_msg = ss.str();
		(*vMeasurements) = (vdnaMsrPtr) (*(_pMsrs));
	}
}

// GPSBaseline_pimpl
//

void GPSBaseline_pimpl::pre()
{
	_dnaGpsBaseline.reset(new CDnaGpsBaseline);
	_dnaGpsBaseline->SetType(_parent_dnaGpsBaselineCluster->GetType());
	_dnaGpsBaseline->SetFirst(_parent_dnaGpsBaselineCluster->GetFirst());
	_dnaGpsBaseline->SetTarget(_parent_dnaGpsBaselineCluster->GetTarget());
	
	_dnaGpsBaseline->SetReferenceFrame(_parent_dnaGpsBaselineCluster->GetReferenceFrame());
	_dnaGpsBaseline->SetEpoch(_parent_dnaGpsBaselineCluster->GetEpoch()); 
	
	_dnaGpsBaseline->SetClusterID(_parent_dnaGpsBaselineCluster->GetClusterID());

	_dnaGpsBaseline->SetClusterDBID(
		_parent_dnaGpsBaselineCluster->GetClusterDBID(),
		_parent_dnaGpsBaselineCluster->GetClusterDBIDset());
	
	if (_parent_dnaGpsBaselineCluster->GetType().empty())
		throw XMLInteropException("\"Type\" element must be provided before \"GPSBaseline\" element.", 0);

	// Is this a 'G' measurement
	if (_parent_dnaGpsBaselineCluster->GetTypeC() != 'X')
		return;

	const UINT32 measCount = _parent_dnaGpsBaselineCluster->GetTotal();

	// Only one baseline?  If so, no co-variances exist.
	// Two or more baselines?  Then reserve co-variance vector
	if (measCount > 1)
		_dnaGpsBaseline->ReserveGpsCovariancesCount(measCount - 1);

}

void GPSBaseline_pimpl::X(const ::std::string& X)
{
	if (_dnaGpsBaseline)
		_dnaGpsBaseline->SetX(X);

	// increment measurement counter
	*(_pMeasurementCount) += 1;

	switch (_dnaGpsBaseline->GetTypeC())
	{
	case 'G':
		g_parsemsr_tally.G++;
		break;
	case 'X':
		g_parsemsr_tally.X++;
		break;
	}
}

void GPSBaseline_pimpl::Y(const ::std::string& Y)
{
	if (_dnaGpsBaseline)
		_dnaGpsBaseline->SetY(Y);

	// increment measurement counter
	*(_pMeasurementCount) += 1;

	switch (_dnaGpsBaseline->GetTypeC())
	{
	case 'G':
		g_parsemsr_tally.G++;
		break;
	case 'X':
		g_parsemsr_tally.X++;
		break;
	}
}

void GPSBaseline_pimpl::Z(const ::std::string& Z)
{
	if (_dnaGpsBaseline)
		_dnaGpsBaseline->SetZ(Z);

	// increment measurement counter
	*(_pMeasurementCount) += 1;

	switch (_dnaGpsBaseline->GetTypeC())
	{
	case 'G':
		g_parsemsr_tally.G++;
		break;
	case 'X':
		g_parsemsr_tally.X++;
		break;
	}
}

void GPSBaseline_pimpl::MeasurementDBID(const ::std::string& MeasurementID)
{
	_dnaGpsBaseline->SetMeasurementDBID(MeasurementID);
	_dnaGpsBaseline->SetClusterDBID(
		_parent_dnaGpsBaselineCluster->GetClusterDBID(),
		_parent_dnaGpsBaselineCluster->GetClusterDBIDset());
}

void GPSBaseline_pimpl::SigmaXX(const ::std::string& SigmaXX)
{
	_dnaGpsBaseline->SetSigmaXX(SigmaXX);
}

void GPSBaseline_pimpl::SigmaXY(const ::std::string& SigmaXY)
{
	_dnaGpsBaseline->SetSigmaXY(SigmaXY);
}

void GPSBaseline_pimpl::SigmaXZ(const ::std::string& SigmaXZ)
{
	_dnaGpsBaseline->SetSigmaXZ(SigmaXZ);
}

void GPSBaseline_pimpl::SigmaYY(const ::std::string& SigmaYY)
{
	_dnaGpsBaseline->SetSigmaYY(SigmaYY);
}

void GPSBaseline_pimpl::SigmaYZ(const ::std::string& SigmaYZ)
{
	_dnaGpsBaseline->SetSigmaYZ(SigmaYZ);
}

void GPSBaseline_pimpl::SigmaZZ(const ::std::string& SigmaZZ)
{
	_dnaGpsBaseline->SetSigmaZZ(SigmaZZ);
}

void GPSBaseline_pimpl::GPSCovariance()
{
}

void GPSBaseline_pimpl::post_GPSBaseline()
{
	if (_dnaGpsBaseline && _parent_dnaGpsBaselineCluster)
		_parent_dnaGpsBaselineCluster->AddGpsBaseline(
		_dnaGpsBaseline.get());
}

// GPSCovariance_pimpl
//

void GPSCovariance_pimpl::pre()
{
	_dnaGpsCovariance.reset(new CDnaCovariance);
	_dnaGpsCovariance->SetType("X");
	_dnaGpsCovariance->SetClusterID(_parent_dnaGpsBaseline->GetClusterID());
}

void GPSCovariance_pimpl::m11(const ::std::string& m11)
{
	_dnaGpsCovariance->SetM11(m11);
}

void GPSCovariance_pimpl::m12(const ::std::string& m12)
{
	_dnaGpsCovariance->SetM12(m12);
}

void GPSCovariance_pimpl::m13(const ::std::string& m13)
{
	_dnaGpsCovariance->SetM13(m13);
}

void GPSCovariance_pimpl::m21(const ::std::string& m21)
{
	_dnaGpsCovariance->SetM21(m21);
}

void GPSCovariance_pimpl::m22(const ::std::string& m22)
{
	_dnaGpsCovariance->SetM22(m22);
}

void GPSCovariance_pimpl::m23(const ::std::string& m23)
{
	_dnaGpsCovariance->SetM23(m23);
}

void GPSCovariance_pimpl::m31(const ::std::string& m31)
{
	_dnaGpsCovariance->SetM31(m31);
}

void GPSCovariance_pimpl::m32(const ::std::string& m32)
{
	_dnaGpsCovariance->SetM32(m32);
}

void GPSCovariance_pimpl::m33(const ::std::string& m33)
{
	_dnaGpsCovariance->SetM33(m33);
}

void GPSCovariance_pimpl::post_GPSCovariance()
{
	if (_dnaGpsCovariance && _parent_dnaGpsBaseline)
		_parent_dnaGpsBaseline->AddGpsCovariance(
		_dnaGpsCovariance.get());
}

// Height_pimpl
//

void Height_pimpl::pre()
{
}

void Height_pimpl::system(const ::std::string& system)
{
	_parent_dnaStn->SetHeightSystem(system);
}

void Height_pimpl::post_Height ()
{
	_parent_dnaStn->SetHeight(post_string());
}

// PointCovariance_pimpl
//

void PointCovariance_pimpl::pre()
{
	_dnaPointCovariance.reset(new CDnaCovariance);
	_dnaPointCovariance->SetType("Y");
	_dnaPointCovariance->SetClusterID(_parent_dnaGpsPoint->GetClusterID());
}

void PointCovariance_pimpl::m11(const ::std::string& m11)
{
	_dnaPointCovariance->SetM11(m11);
}

void PointCovariance_pimpl::m12(const ::std::string& m12)
{
	_dnaPointCovariance->SetM12(m12);
}

void PointCovariance_pimpl::m13(const ::std::string& m13)
{
	_dnaPointCovariance->SetM13(m13);
}

void PointCovariance_pimpl::m21(const ::std::string& m21)
{
	_dnaPointCovariance->SetM21(m21);
}

void PointCovariance_pimpl::m22(const ::std::string& m22)
{
	_dnaPointCovariance->SetM22(m22);
}

void PointCovariance_pimpl::m23(const ::std::string& m23)
{
	_dnaPointCovariance->SetM23(m23);
}

void PointCovariance_pimpl::m31(const ::std::string& m31)
{
	_dnaPointCovariance->SetM31(m31);
}

void PointCovariance_pimpl::m32(const ::std::string& m32)
{
	_dnaPointCovariance->SetM32(m32);
}

void PointCovariance_pimpl::m33(const ::std::string& m33)
{
	_dnaPointCovariance->SetM33(m33);
}

void PointCovariance_pimpl::post_PointCovariance()
{
	if (_dnaPointCovariance && _parent_dnaGpsPoint)
		_parent_dnaGpsPoint->AddPointCovariance(
		_dnaPointCovariance.get());
}

// StationCoord_pimpl
//

void StationCoord_pimpl::pre()
{
}

void StationCoord_pimpl::Name(const ::std::string& /*Name*/)
{
	//_parent_dnaStn->SetName(Name);
}

void StationCoord_pimpl::XAxis(const ::std::string& XAxis)
{
	_parent_dnaStn->SetXAxis(XAxis);
}

void StationCoord_pimpl::YAxis(const ::std::string& YAxis)
{
	_parent_dnaStn->SetYAxis(YAxis);
}

void StationCoord_pimpl::Height(const ::std::string& Height)
{
	_parent_dnaStn->SetHeight(Height);
}

void StationCoord_pimpl::HemisphereZone(const ::std::string& HemisphereZone)
{
	_parent_dnaStn->SetHemisphereZone(HemisphereZone);
}

void StationCoord_pimpl::GeoidModel()
{
}



// GeoidModel_pimpl
//

void GeoidModel_pimpl::pre()
{
}

void GeoidModel_pimpl::NValue (const ::std::string& /*NValue*/)
{

}

void GeoidModel_pimpl::DeflPM (const ::std::string& /*DeflPM*/)
{

}

void GeoidModel_pimpl::DeflPV (const ::std::string& /*DeflPV*/)
{

}

void GeoidModel_pimpl::post_GeoidModel()
{
}

void StationCoord_pimpl::post_StationCoord()
{
}

// type_pimpl
//

void type_pimpl::pre()
{
}

void type_pimpl::post_type()
{
	//const ::std::string& v(post_string());
}

// referenceframe_pimpl
//

void referenceframe_pimpl::pre()
{
	
}

// Note that if the DnaXmlFormat element in the XML file does not contain an
// attribute named referenceframe, the default will be set to whatever is in
// the XSD file.
// For DynaML.xsd, the following is specified:
//     <xs:attribute name="referenceframe" type="xs:string" use="optional" default="GDA2020"/>
// In this case, post_string() will return "GDA2020"
void referenceframe_pimpl::post_type(string& referenceframe, bool userspecifiedreferenceframe, bool overridereferenceframe)
{
	// 1. Get the DnaXmlFormat referenceframe attribute value from the file
	_referenceframe = trimstr(post_string());

	// 2. Does the user want to override the default datum?
	if (overridereferenceframe)
		// Do nothing, just return as referenceframe will become 
		// the default for all stations and measurements loaded
		// from the file.
		return;
	
	// 3. Does the user want the referenceframe attribute in the file to become the default?
	if (!userspecifiedreferenceframe)
	{
		// Here, the assumption is, if the user did not specify a reference frame,
		// then the user wants to adopt the DnaXmlFormat referenceframe attribute
		
		// But, is this attribute value an empty string?  As long as a default value 
		// is specified in DynaML.xsd, this value will never be empty, unless the user
		// has inadvertently set in the xml file, e.g.:
		//  <DnaXmlFormat referenceframe="" ... >
		if (_referenceframe.empty())
			// Set to the default reference frame passed from import
			_referenceframe = referenceframe;
		else
			// adopt the DnaXmlFormat referenceframe attribute 
			referenceframe = _referenceframe;
	}
}

// epoch_pimpl
//

void epoch_pimpl::pre()
{
}

// Note that if the DnaXmlFormat element in the XML file does not contain an
// attribute named epoch, the default will be set to whatever is in the XSD file.
// For DynaML.xsd, the following is specified:
//     <xs:attribute name="epoch" type="xs:string" use="optional" default="01.01.1994"/>
// In this case, post_string() will return "01.01.1994"
void epoch_pimpl::post_type(string& epoch, bool overridereferenceframe)
{
	// 1. Get the DnaXmlFormat epoch attribute value from the file
	_epoch = post_string();

	// 2. Does the user want to override the default datum?
	if (overridereferenceframe)
		// Do nothing, just return as epoch will become 
		// the default for all stations and measurements loaded
		// from the file.
		return;

	// Since import doesn't offer an option to capture epoch on the command line,
	// take the epoch from the DnaXmlFormat referenceframe attribute 
	if (_epoch.empty())
		epoch = _epoch;	
}

// system_pimpl
//

void system_pimpl::pre()
{
}

void system_pimpl::post_system ()
{
	const ::std::string& v (post_string());

}
