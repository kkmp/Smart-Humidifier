import { Fragment } from "react";
import { ClipLoader } from "react-spinners";


const LoadingComponent = ({size}) => {
  return (
    <Fragment>
      <div>
        <ClipLoader size={size} color="#1076eb"/>
      </div>
    </Fragment>
  );
};

export default LoadingComponent;