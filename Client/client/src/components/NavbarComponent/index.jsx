import React, { Fragment } from 'react'
import { BsCalendar, BsDropletFill, BsGear, BsHouse } from 'react-icons/bs'


function NavbarComponent({ selectedButton, onButtonChange }) {

  const handleButtonClick = (option) => {
    onButtonChange(option)
  };

  return (
    <Fragment>
      <div className="sidebar">
        <ul className="nav flex-column">
          <li className="nav-item">
            <div className="nav-item-link">
              <div className="nav-link-tile logo">
                <div className='nav-link-icon'>
                  <BsDropletFill size={30} color="white" />
                </div>
                <div className='nav-link-text'>
                  ESP-Humidifier
                </div>
              </div>
            </div>
          </li>
          <li className="nav-item">
            <div className="nav-item-link" href="#">
              <button className={`nav-link-tile hoverable ${selectedButton === 'home' ? 'selected' : ''}`} onClick={() => handleButtonClick('home')}>
                <div className='nav-link-icon'>
                  <BsHouse size={30} />
                </div>
                <div className='nav-link-text'>
                  Dashboard
                </div>
              </button>
            </div>
          </li>
          <li className="nav-item">
            <div className="nav-item-link" href="#">
              <button className={`nav-link-tile hoverable ${selectedButton === 'history' ? 'selected' : ''}`} onClick={() => handleButtonClick('history')}>
                <div className='nav-link-icon'>
                  <BsCalendar size={30} />
                </div>
                <div className='nav-link-text'>
                  History
                </div>
              </button>
            </div>
          </li>
          <li className="nav-item">
            <div className="nav-item-link" href="#">
              <button className={`nav-link-tile hoverable ${selectedButton === 'settings' ? 'selected' : ''}`} onClick={() => handleButtonClick('settings')}>
                <div className='nav-link-icon'>
                  <BsGear size={30} />
                </div>
                <div className='nav-link-text'>
                  Settings
                </div>
              </button>
            </div>
          </li>
        </ul>
      </div>
    </Fragment>
  );
}

export default NavbarComponent;
