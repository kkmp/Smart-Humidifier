import { Fragment, useState, useLayoutEffect, useEffect, useRef } from "react";
import axios from "axios";
import { Button } from "react-bootstrap";
import 'bootstrap/dist/css/bootstrap.min.css';
import { BsThermometerSun, BsPower } from "react-icons/bs";
import { WiHumidity } from "react-icons/wi"
import { FaHandHoldingWater, FaCheckCircle } from "react-icons/fa"
import { MdHdrAuto } from "react-icons/md"
import { SiTailwindcss } from "react-icons/si"
import LoadingComponent from "../LoadingComponent";

const HomeComponent = ({ espIp }) => {
    const [data, setData] = useState(null)
    const [isOn, setIsOn] = useState(false)
    const [automode, setAutomode] = useState(true)
    const [manualmode, setManualmode] = useState(false)
    const [minHumidity, setMinHumidity] = useState(50)
    const [focus, setFocus] = useState(false)
    const focusRef = useRef(focus)

    useEffect(() => {
        if (automode) {
            setMinHumidity(50)
        }
    }, [automode])

    useEffect(() => {
        focusRef.current = focus
    }, [focus]);

    useLayoutEffect(() => {
        checkIsOn()

        const intervalIdMeasurement = setInterval(makeMeasurement, 5000)
        const intervalIdIsOn = setInterval(checkIsOn, 5000)
        const intervalIdIsAutomode = setInterval(checkIsAutomode, 5000)
        const intervalIdParameters = setInterval(checkParameters, 5000)

        return () => {
            clearInterval(intervalIdMeasurement)
            clearInterval(intervalIdIsOn)
            clearInterval(intervalIdIsAutomode)
            clearInterval(intervalIdParameters)
        };
    }, [])

    const makeMeasurement = async () => {
        try {
            const response = await axios.get(`http://${espIp}/getMeasurement`)
            setData(response.data)
        } catch (error) {
            console.error('An error occurred while downloading data:', error)
        }
    }

    const checkIsOn = async () => {
        try {
            const response = await axios.get(`http://${espIp}/isOn`)
            setIsOn(response.data.isOn)
        } catch (error) {
            console.error('An error occurred while downloading data:', error)
        }
    }

    const checkIsAutomode = async () => {
        try {
            const response = await axios.get(`http://${espIp}/isAutomode`)
            setAutomode(response.data.isAutomode)
            setManualmode(!response.data.isAutomode)
        } catch (error) {
            console.error('An error occurred while downloading data:', error)
        }
    }

    const checkParameters = async () => {
        if (focusRef.current) {
            return;
        }

        try {
            const response = await axios.get(`http://${espIp}/getParameters`)
            setMinHumidity(response.data.minHumidity)
        } catch (error) {
            console.error('An error occurred while downloading data:', error)
        }
    }

    const piezoToggle = async () => {
        try {
            const response = await axios.get(`http://${espIp}/piezoToggle?power=` + (isOn ? 0 : 1))
            setIsOn(response.data.isOn)
        } catch (error) {
            console.error('An error occurred while downloading data:', error)
        }
    }

    const setMode = async (mode) => {
        try {
            const response = await axios.get(`http://${espIp}/piezoMode?mode=` + (mode ? 1 : 0))
            setAutomode(response.data.isAutomode)
            setManualmode(!response.data.isAutomode)
        } catch (error) {
            console.error('An error occurred while downloading data:', error)
        }
    }

    const setParameters = async () => {
        try {
            const response = await axios.get(`http://${espIp}/setParameters?minHumidity=${minHumidity}`)
            setMinHumidity(response.data.minHumidity)
        } catch (error) {
            console.error('An error occurred while downloading data:', error)
        }
    }

    const handleChange = () => {
        piezoToggle()
    }

    const handleAutomode = () => {
        setMode(true)
    }

    const handleManualmode = () => {
        setMode(false)
    }

    const handleOnFocus = () => {
        setFocus(true)
    }

    const handleOnBlur = () => {
        setFocus(false)
    }

    const handleOnClickParameters = () => {
        if (minHumidity === "" || minHumidity === null) {
            alert("Missing data!")
        } else if (minHumidity < 0) {
            alert("Incorrect value of minimum humidity!")
        } else {
            setParameters()
            alert("The values have been set!")
        }
    }

    return (
        <Fragment>
            <div>
                <div className="box-row">
                    <div className="box-parameters">
                        {data !== null ?
                            <Fragment>
                                <div className="box-col">
                                    <div className="box-icon">
                                        <WiHumidity size={150} />
                                    </div>
                                    <div className="box-text">
                                        {data.Humidity.toFixed(2)}%
                                    </div>
                                </div>
                                <div className="box-col">
                                    <div className="box-icon">
                                        <BsThermometerSun size={110} />
                                    </div>
                                    <div className="box-text">
                                        {data.Temperature.toFixed(2)}°C
                                    </div>
                                </div>
                                <div className="box-col">
                                    <div className="box-icon">
                                        <SiTailwindcss size={100} />
                                    </div>
                                    <div className="box-text">
                                        {data.Pressure.toFixed(2)} hPa
                                    </div>
                                </div>
                            </Fragment> : <div className="spinner-area">
                                <div className="spinner-object">
                                    <LoadingComponent size={50} />
                                </div>
                                <div className="box-text">
                                    Measuring. Please wait...
                                </div>
                            </div>}
                    </div>
                </div>
            </div>
            <div className="row-button">
                <Button
                    variant={isOn ? 'success' : 'danger'}
                    className={`toggle-button button-power ${isOn ? 'toggle-on' : 'toggle-off'}`}
                    onClick={handleChange}
                    id="toggle"
                >
                    <BsPower size={120} /> 
                </Button>
                <div className="row-text">
                    {isOn ? "Power is on" : "Power is off"}
                </div>
            </div>
            {isOn ? <Fragment>
                <div className="row-many-buttons">
                    <div className="button-with-text">
                        <Button onClick={handleAutomode} disabled={automode}><MdHdrAuto size={120} /></Button>
                        <div className="row-text">
                            Automatic mode
                        </div>
                    </div>
                    <div className="button-with-text">

                        <Button onClick={handleManualmode} disabled={manualmode}><FaHandHoldingWater size={120} /></Button>
                        <div className="row-text">
                            Manual mode
                        </div>
                    </div>
                </div>
                <div className="row-many-buttons">
                    <div className="input-text">
                        Target humidity
                        <div>
                            <input
                                type="number"
                                value={minHumidity}
                                onChange={(event) => setMinHumidity(event.target.value)}
                                min={0}
                                max={100}
                                disabled={automode}
                                onFocus={handleOnFocus}
                                onBlur={handleOnBlur}
                            /> %
                        </div>
                    </div>
                    <Button disabled={automode} onClick={handleOnClickParameters}>
                        <FaCheckCircle size={30} />
                    </Button>
                </div>
            </Fragment> : null}
        </Fragment>
    );
};

export default HomeComponent;